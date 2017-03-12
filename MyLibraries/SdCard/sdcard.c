/**
 * @file    sdcard.c
 * @brief   SD card control functions.
 * @date    22 kwi 2014
 * @author  Michal Ksiezopolski
 * 
 * @verbatim
 * Copyright (c) 2014 Michal Ksiezopolski.
 * All rights reserved. This program and the 
 * accompanying materials are made available 
 * under the terms of the GNU Public License 
 * v3.0 which accompanies this distribution, 
 * and is available at 
 * http://www.gnu.org/licenses/gpl.html
 * @endverbatim
 */

#include "sdcard.h"
#include "spi_hal.h"
#include "timers.h"
#include "utils.h"
#include <stdio.h>

/**
 * @addtogroup SD_CARD
 * @{
 */

#ifndef DEBUG
  #define DEBUG
#endif

#ifdef DEBUG
  #define print(str, args...) printf(""str"%s",##args,"")
  #define println(str, args...) printf("SD--> "str"%s",##args,"\r\n")
#else
  #define print(str, args...) (void)0
  #define println(str, args...) (void)0
#endif

/**
 * @brief SD commands (SPI command subset) as per SanDisk Secure Digital Card
 * product manual.
 */
typedef enum {
  SD_GO_IDLE_STATE           = 0 ,  ///< Resets SD Card.
  SD_SEND_OP_COND            = 1 ,  ///< Activates the card initialization process, sends host capacity.
  SD_SEND_IF_COND            = 8 ,  ///< Asks card whether it can operate in given voltage range.
  SD_SEND_CSD                = 9 ,  ///< Ask for card specific data (CSD).
  SD_SEND_CID                = 10,  ///< Ask for card identification (CID).
  SD_STOP_TRANSMISSION       = 12,  ///< Forces a card to stop transmission during a multiple block read operation.
  SD_SEND_STATUS             = 13,  ///< Ask for status register contents.
  SD_SET_BLOCKLEN            = 16,  ///< Selects block length in bytes for all following block commands
  SD_READ_SINGLE_BLOCK       = 17,  ///< Reads a block of size set by SET_BLOCKLEN
  SD_READ_MULTIPLE_BLOCK     = 18,  ///< Continuously transfers data blocks from card to host until interrupted by STOP_TRANSMISSION
  SD_WRITE_BLOCK             = 24,  ///< Writes a block of size set by SET_BLOCKLEN
  SD_WRITE_MULTIPLE_BLOCK    = 25,  ///< Continuously writes blocks of data until a stop transmission token is sent
  SD_PROGRAM_CSD             = 27,  ///< Programs the programmable bits of CSD
  SD_ERASE_WR_BLK_START_ADDR = 32,  ///< Sets the address of the first write block to be erased
  SD_ERASE_WR_BLK_END_ADDR   = 33,  ///< Sets the address of the last write block of the continuous range to be erased
  SD_ERASE                   = 38,  ///< Erases all previously selected write blocks
  SD_APP_CMD                 = 55,  ///< Next command is application specific command
  SD_READ_OCR                = 58,  ///< Reads OCR register
  SD_CRC_ON_OFF              = 59,  ///< Turns CRC on or off
} SD_CommandTypedef;
/*
 * Application specific commands, ACMD
 */
#define SD_ACMD_SEND_OP_COND        41  ///< Activates the card initialization process, sends host capacity.
#define SD_ACMD_SEND_SCR            51  ///< Reads SD Configuration register
#define SD_SEND_NUM_WR_BLOCKS       22  ///< Gets number of well written blocks
/*
 * Other SD defines
 */
#define SD_IF_COND_CHECK  0xaa    ///< Check pattern for SEND_IF_COND command
#define SD_IF_COND_VOLT   (1<<8)  ///< Signifies voltage range 2.7-3.6V
#define SD_ACMD41_HCS     (1<<30) ///< Host can handle SDSC and SDHC cards
/*
 * Control tokens
 */
#define SD_TOKEN_SBR_MBR_SBW  0xfe ///< Start block for single block read, multiple block read, single block write. This token is sent, then 2-513 bytes of data, two bytes CRC
#define SD_TOKEN_MBW_START    0xfc ///< Start block token for multiple block write - data will be transferred
#define SD_TOKEN_MBW_STOP     0xfd ///< Stop transmission token for multiple block write
/*
 * Every data block sent to SD card will be acknowledged by data response token.
 * In case of error during Multiple Block Write host shall stop transmission
 * using CMD12. ACMD22 may be used to find number of well written blocks.
 * CMD13 may be sent to get cause of write problem
 */
#define SD_TOKEN_DATA_ACCEPTED  0x05 ///< Data accepted
#define SD_TOKEN_DATA_CRC       0x0b ///< Data rejected due to CRC error
#define SD_TOKEN_DATA_WRITE_ERR 0x0d ///< Data rejected due to write error

static Boolean isSDHC;            ///< Is the card SDHC?
static uint64_t cardCapacity;     ///< Capacity of SD card in bytes
static Boolean isCardInIdleState; ///< Is card in IDLE state
static Boolean isCardInitalized;  ///< Is the card initalized

/**
 * @brief SD Card R1 response structure
 * @details This token is sent after every command
 * with the exception of SEND_STATUS command
 */
typedef union {

  struct {
    uint8_t inIdleState         :1; ///< The card is in IDLE state
    uint8_t eraseReset          :1; ///< Erase sequence was cleared before executing because an out of erase sequence commands was received
    uint8_t illegalCommand      :1; ///< Illegal command code detected
    uint8_t commErrorCRC        :1; ///< CRC check of last command failed
    uint8_t eraseSequenceError  :1; ///< Error in sequence of erase commands
    uint8_t addressErrror       :1; ///< Misaligned address didn't match block length used in command
    uint8_t parameterError      :1; ///< Command's argument was outside the range allowed for the card
    uint8_t reserved            :1; ///< Reserved (always 0)
  } flags;

  uint8_t asUint8; ///< R1 response fields as byte
} SD_ResponseR1;
/**
 * @brief SD Card R2 response structure
 * @details This token is sent in response to SEND_STATUS command.
 */
typedef union {

  struct {
    uint16_t cardLocked          :1; ///< Set when card is locked bu user
    uint16_t wpEraseSkip         :1; ///< Set when host attempts to write a write-protected sector or makes errors during card lock/unlock operation
    uint16_t error               :1; ///< General or unknown error occured
    uint16_t errorCC             :1; ///< Internal card controller error
    uint16_t cardFailedECC       :1; ///< Card internal ECC was applied but failed to correct data
    uint16_t wpViolation         :1; ///< Command tried to write a write-protected block
    uint16_t eraseParam          :1; ///< Invalid selection for erase, sectors, groups.
    uint16_t outOfRange          :1; ///<
    uint16_t inIdleState         :1; ///< The card is in IDLE state
    uint16_t eraseReset          :1; ///< Erase sequence was cleared before executing because an out of erase sequence commands was received
    uint16_t illegalCommand      :1; ///< Illegal command code detected
    uint16_t commErrorCRC        :1; ///< CRC check of last command failed
    uint16_t eraseSequenceError  :1; ///< Error in sequence of erase commands
    uint16_t addressErrror       :1; ///< Misaligned address didn't match block length used in command
    uint16_t parameterError      :1; ///< Command's argument was outside the range allowed for the card
    uint16_t reserved            :1; ///< Reserved (always 0)
  } flags;

  uint16_t asUint16; ///< R1 response fields as byte

} SD_ResponseR2;
/**
 * @brief Operation conditions register
 */
typedef union {

  struct {
    uint32_t reserved   :15;
    uint32_t volt27to28 :1;
    uint32_t volt28to29 :1;
    uint32_t volt29to30 :1;
    uint32_t volt30to31 :1;
    uint32_t volt31to32 :1;
    uint32_t volt32to33 :1;
    uint32_t volt33to34 :1;
    uint32_t volt34to35 :1;
    uint32_t volt35to36 :1;
    uint32_t switchingTo18      :1;
    uint32_t reserved2          :5;
    uint32_t cardCapacityStatus :1; ///< 0 - SDSC, 1 - SDHC, valid only after power up bit is 1
    uint32_t cardPowerUpStatus  :1; ///< Set to 0 if card has not finished power up routine
  } bits;

  uint32_t asUint32;

} SD_OCR;
/**
 * @brief Card Identification Register
 */
typedef struct {

  uint8_t manufacturerID;
  uint8_t applicationID[2];
  uint8_t name[5];
  uint8_t revision;
  uint8_t serial[4];
  uint8_t manufacturingDate[2];
  uint8_t crc;

} __attribute((packed)) SD_CID;
/**
 * @brief Card specific data register
 * @details The data comes MSB first. In C in a bit array corresponds
 * to the least significant bit.
 *
 * TODO Add version of this register for SDSC cards
 */
typedef struct {

  // fourth uint32_t
    uint32_t reserved1   :1;
    uint32_t crc :7;
    uint32_t reserved2 :2;
    uint32_t fileFormat :2;
    uint32_t tmpWrtProtect :1;
    uint32_t permWrtProtect :1;
    uint32_t copyFlag :1;
    uint32_t fileFmtGrp :1;

    uint32_t reserved3 :5;
    uint32_t partialBlkWrt :1;
    uint32_t maxWrtBlkLen :4;
    uint32_t wrtSpeed :3;
    uint32_t reserved4 :2;
    uint32_t wrtProtectEna :1;

  // third uint32_t
    uint32_t wrtProtectSize :7;
    uint32_t sectorSize :7;
    uint32_t eraseSingleBlkEna :1;
    uint32_t reserved5 :1;
    uint32_t deviceSize :22; ///< Capacity of the card

  // second uint32_t
    uint32_t reserved6 : 6;
    uint32_t DSR :1;
    uint32_t rdBlkMisalign :1;
    uint32_t wrtBlkMisalign :1;
    uint32_t rdBlkPartial :1;

    uint32_t maxReadLen :4;
    uint32_t cardCommandClass :12;

  // first uint32_t
    uint32_t maxDataRate :8;
    uint32_t dataRdCycles:8;
    uint32_t dataRdTime :8;
    uint32_t reserved7 :6;
    uint32_t csdType :2;  ///< Type of the structure

} __attribute((packed)) SD_CSD;

static SD_CardErrorsTypedef sendCommand(uint8_t cmd, uint32_t args);
static void getResponseR3orR7(uint8_t* responseBuffer);
static SD_CardErrorsTypedef readOcr(SD_OCR* asUint32);
static SD_CardErrorsTypedef readCid(SD_CID* cid);
static SD_CardErrorsTypedef readCsd(SD_CSD* csd);

#define DUMMY_BYTE 0xff ///< Dummy byte for reading data
#define NO_ERRORS_IN_IDLE_STATE   0x01
#define NO_ERRORS_LEFT_IDLE_STATE 0x00

/**
 * @brief Initialize the SD card.
 * @details This function initializes both SDSC and SDHC cards.
 * It uses low-level SPI functions.
 */
int SD_Initialize(void) {

  const int BUFFER_LENGTH = 10;
  uint8_t sdCommandsBuffer[BUFFER_LENGTH];
  SD_CardErrorsTypedef result;

  SpiHal_initialize(SPI_HAL_SPI1);
  SpiHal_select(SPI_HAL_SPI1);

  // Synchronize card with SPI
  const int SYNCHRONIZATION_BYTES = 20;
  for (int i = 0; i < SYNCHRONIZATION_BYTES; i++) {
    SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE);
  }

  isCardInIdleState = TRUE;

  // send CMD0
  sendCommand(SD_GO_IDLE_STATE, 0);
  // send CMD8
  sendCommand(SD_SEND_IF_COND,
      SD_IF_COND_VOLT | SD_IF_COND_CHECK); // voltage range and check pattern
  // CMD8 gets more info
  getResponseR3orR7(sdCommandsBuffer);

  // Check if card supports given voltage range
  if ((sdCommandsBuffer[3] != SD_IF_COND_CHECK) ||
      (sdCommandsBuffer[2] != (SD_IF_COND_VOLT>>8))) {
    println("SEND_IF_COND error");
    for (int i=0; i<4; i++) {
      print("%02x ", sdCommandsBuffer[i]);
    }
    print("\r\n");

  }

  // CMD58
  SD_OCR ocr;
  readOcr(&ocr);;

  // Send ACMD41 until card goes out of IDLE state
  const int MAXIMUM_ACMD41_TRIES = 10;
  const int SD_INITIAL_DELAY = 20;
  isCardInIdleState = FALSE;
  for (int i = 0; i < MAXIMUM_ACMD41_TRIES; i++) {
    result = sendCommand(SD_APP_CMD, 0);
    result = sendCommand(SD_ACMD_SEND_OP_COND, SD_ACMD41_HCS);
    // Without this delay card wouldn't initialize the first time after
    // power was connected.
    Timer_delayMillis(SD_INITIAL_DELAY);

    if (result == SD_NO_ERROR) { // Card left IDLE state and no errors
      break;
    }

    if (i == MAXIMUM_ACMD41_TRIES - 1) {
      println("Failed to initialize SD card");
      return SD_INIT_FAILED;
    }
  }

  SD_CID cid;
  readCid(&cid);
  SD_CSD csd;
  readCsd(&csd);
  // Read Card Capacity Status - SDSC or SDHC?
  readOcr(&ocr);

  // check capacity
  if (ocr.bits.cardCapacityStatus == TRUE) {
    println("SDHC card connected");
    isSDHC = TRUE;
  } else {
    println("SDSC card connected");
    isSDHC = FALSE;
  }

  SpiHal_deselect(SPI_HAL_SPI1);
  isCardInitalized = TRUE;
  return SD_NO_ERROR;

}
/**
 * @brief Gets the capacity of the card.
 * @return Card capacity in bytes.
 */
uint64_t SD_ReadCapacity(void) {
  return cardCapacity;
}
/**
 * @brief Read sectors from SD card
 * @param readDataBuffer Data buffer
 * @param startSector Start sector
 * @param sectorsToRead Number of sectors to read
 * @retval SD_NO_ERROR Read was successful
 * @retval SD_BLOCK_READ_ERROR Error occurred
 */
int SD_ReadSectors(uint8_t* readDataBuffer, uint32_t startSector,
    uint32_t sectorsToRead) {

  if (!isCardInitalized) {
    return SD_CARD_NOT_INITALIZED;
  }

  const int NUMBER_OF_BYTES_IN_SECTOR = 512;
  SD_CardErrorsTypedef result;

  // SDSC cards use byte addressing, SDHC use block addressing
  if (!isSDHC) {
    startSector *= NUMBER_OF_BYTES_IN_SECTOR;
  }

  SpiHal_select(SPI_HAL_SPI1);

  result = sendCommand(SD_READ_MULTIPLE_BLOCK, startSector);

  if (result != SD_NO_ERROR) {
    println("SD_READ_MULTIPLE_BLOCK error");
    SpiHal_deselect(SPI_HAL_SPI1);
    return SD_BLOCK_READ_ERROR;
  }

  while (sectorsToRead) {
    // wait for data token
    while (SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE) != SD_TOKEN_SBR_MBR_SBW);
    SpiHal_readBuffer(SPI_HAL_SPI1, readDataBuffer, NUMBER_OF_BYTES_IN_SECTOR);
    SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE);
    SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE); // two bytes CRC
    sectorsToRead--;
    readDataBuffer += NUMBER_OF_BYTES_IN_SECTOR; // move buffer pointer forward
  }

  sendCommand(SD_STOP_TRANSMISSION, 0);

  // R1b response - check busy flag
  while(!SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE));

  SpiHal_deselect(SPI_HAL_SPI1);

  return SD_NO_ERROR;
}
/**
 * @brief Write sectors to SD card
 * @param buf Data buffer
 * @param sector First sector to write
 * @param count Number of sectors to write
 * @retval 0 Read was successful
 * @retval 1 Error occurred
 */
int SD_WriteSectors(uint8_t* writeDataBuffer, uint32_t startSector,
    uint32_t sectorsToWrite) {

  if (!isCardInitalized) {
    return SD_CARD_NOT_INITALIZED;
  }

  const int NUMBER_OF_BYTES_IN_SECTOR = 512;
  const uint8_t START_BLOCK_TOKEN = 0xfc;
  const uint8_t STOP_TRANSMISSION_TOKEN = 0xfd;
  SD_CardErrorsTypedef result;

  // SDSC cards use byte addressing, SDHC use block addressing
  if (!isSDHC) {
    startSector *= NUMBER_OF_BYTES_IN_SECTOR;
  }

  SpiHal_select(SPI_HAL_SPI1);

  result = sendCommand(SD_WRITE_MULTIPLE_BLOCK, startSector);

  if (result != SD_NO_ERROR) {
    println("SD_WRITE_MULTIPLE_BLOCK error");
    SpiHal_deselect(SPI_HAL_SPI1);
    return SD_BLOCK_WRITE_ERROR;
  }

  while (sectorsToWrite) {
    SpiHal_transmitByte(SPI_HAL_SPI1, START_BLOCK_TOKEN);
    SpiHal_sendBuffer(SPI_HAL_SPI1, writeDataBuffer, NUMBER_OF_BYTES_IN_SECTOR);
    SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE);
    SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE); // two bytes CRC
    sectorsToWrite--;
    writeDataBuffer += NUMBER_OF_BYTES_IN_SECTOR; // move buffer pointer forward
    // data response
    SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE);
    while(!SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE)); // wait while card is busy
  }

  SpiHal_transmitByte(SPI_HAL_SPI1, STOP_TRANSMISSION_TOKEN); // stop transmission token
  SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE);
  while(!SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE)); // wait while card is busy

  SpiHal_deselect(SPI_HAL_SPI1);

  return SD_NO_ERROR;
}
/**
 * @brief Reads OCR register
 *
 * @details Returns response because it is called before and after
 * leaving the IDLE state, so we can't check response in the function.
 * Check the response externally.
 *
 * @return OCR register value
 */
SD_CardErrorsTypedef readOcr(SD_OCR* ocr) {
  const int OCR_LENGTH = 4;
  uint8_t ocrBuffer[OCR_LENGTH];
  sendCommand(SD_READ_OCR, 0);
  // OCR has more data
  getResponseR3orR7(ocrBuffer);

  // SD sends this commands MSB first
  // so reverse byte order
  uint32_t* ptr = (uint32_t*)ocr;
  uint32_t* ptrBuf = (uint32_t*)ocrBuffer;

  *ptr = Utils_convertUnsignedIntToHostEndianness(*ptrBuf);

  // Send OCR to terminal
  print("OCR value: ");
  for (int i = 0; i < OCR_LENGTH; i++) {
    print("%02x ", ocrBuffer[i]);
  }
  print("\r\n");

  return SD_NO_ERROR;
}
/**
 * @brief Read CID register of SD card
 * @param cid Structure for filling CID register.
 */
SD_CardErrorsTypedef readCid(SD_CID* cid) {

  const int CID_LENGTH = 16;
  uint8_t cidBuffer[CID_LENGTH];

  sendCommand(SD_SEND_CID, 0);

  // Read CID implemented as read block
  // So do the same as for read block
  // wait for data token
  while (SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE) != SD_TOKEN_SBR_MBR_SBW);
  SpiHal_readBuffer(SPI_HAL_SPI1, cidBuffer, CID_LENGTH);
  SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE);
  SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE); // two bytes CRC

  uint8_t* ptr = (uint8_t*)cid;
  for (int i = 0; i < CID_LENGTH; i++) {
    ptr[i] = cidBuffer[i];
  }

//  UTILS_HexdumpWithCharacters(cidBuffer, CID_LENGTH);

  // R1b response - check busy flag
  while(!SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE));

  return SD_NO_ERROR;
}
/**
 * @brief Read CSD register of SD card
 *
 * @details This function also sets the cardCapacity
 * variable holding the capacity of the card in bytes.
 *
 * @param csd Structure for filling CSD register.
 */
SD_CardErrorsTypedef readCsd(SD_CSD* csd) {
  const int CSD_LENGTH = 16;
  const int CSD_LENGTH_WORDS = 4;
  const int BLOCK_SIZE = 512 * 1024;
  uint8_t csdBuffer[CSD_LENGTH];

  sendCommand(SD_SEND_CSD, 0);

  // Read CID implemented as read block
  // So do the same as for read block
  // wait for data token
  while (SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE) != SD_TOKEN_SBR_MBR_SBW);
  SpiHal_readBuffer(SPI_HAL_SPI1, csdBuffer, CSD_LENGTH);
  SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE);
  SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE); // two bytes CRC

  uint32_t* ptr = (uint32_t*)csd;
  uint32_t* ptrBuf = (uint32_t*)csdBuffer;

  for (int i = 0; i < CSD_LENGTH_WORDS; i++) {
    ptr[i] = Utils_convertUnsignedIntToHostEndianness(ptrBuf[3-i]);
  }

//  UTILS_HexdumpWithCharacters(csdBuffer, CSD_LENGTH);

  println("CSD type: 0x%02x", (unsigned int) csd->csdType);
  println("CSD device size: %u", (unsigned int) csd->deviceSize);

  // size counted in blocks of 512K
  cardCapacity = csd->deviceSize * BLOCK_SIZE;
  println("Card capacity: %u", (unsigned int)cardCapacity);

  // R1b response - check busy flag
  while(!SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE));
  return SD_NO_ERROR;
}
/**
 * @brief Sends a command to the SD card.
 *
 * @details This function works for commands which return 1 byte
 * response - R1 response token. These commands are in the majority.
 *
 * @param cmd Command to send
 * @param args Command arguments: 4 bytes as a 32-bit number
 * @return Returns R1 response token
 */
SD_CardErrorsTypedef sendCommand(uint8_t cmd, uint32_t args) {

  SpiHal_transmitByte(SPI_HAL_SPI1, 0x40 | cmd);
  SpiHal_transmitByte(SPI_HAL_SPI1, args >> 24); // MSB first
  SpiHal_transmitByte(SPI_HAL_SPI1, args >> 16);
  SpiHal_transmitByte(SPI_HAL_SPI1, args >> 8);
  SpiHal_transmitByte(SPI_HAL_SPI1, args);

  // CRC is irrelevant while using SPI interface - only checked for some commands.
  switch (cmd) {
  case SD_GO_IDLE_STATE:
    SpiHal_transmitByte(SPI_HAL_SPI1, 0x95);
    break;
  case SD_SEND_IF_COND:
    SpiHal_transmitByte(SPI_HAL_SPI1, 0x87);
    break;
  default:
    SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE);
  }
  // Practice has shown that a valid response token
  // is sent as the second byte by the card.
  // So, we send a dummy byte first.
  SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE);
  SD_ResponseR1 commandResponse;
  commandResponse.asUint8 = SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE);
//  println("Response to cmd %d is %02x", cmd, commandResponse.asUint8);

  // Check response errors
  uint8_t okResponse;
  if (isCardInIdleState) {
    okResponse = NO_ERRORS_IN_IDLE_STATE;
  } else {
    okResponse = NO_ERRORS_LEFT_IDLE_STATE;
  }

  if (commandResponse.asUint8 != okResponse) {
//    println("Commands %d error", cmd);
    return SD_RESPONSE_ERROR;
  }

  return SD_NO_ERROR;
}
/**
 * @brief Get R3 or R7 response from card
 * @details R3 response is for READ_OCR command (it is actually five bytes R1
 * + 4 bytes of OCR read by this function). R7 is for SEND_IF_COND command
 * (also R1 + 4 bytes containing voltage information)
 * @param responseBuffer Buffer for response
 */
void getResponseR3orR7(uint8_t* responseBuffer) {
  const int RESPONSE_R3_OR_R7_LENGTH = 4;
  for (int i = 0; i < RESPONSE_R3_OR_R7_LENGTH; i++) {
    responseBuffer[i] = SpiHal_transmitByte(SPI_HAL_SPI1, DUMMY_BYTE);
  }
}
/**
 * @}
 */
