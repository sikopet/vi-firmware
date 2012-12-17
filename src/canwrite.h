#ifndef _CANWRITE_H_
#define _CANWRITE_H_

#include "canutil.h"

/* Public: Encode and store value in a bit field for the given signal.
 *
 * The value is converted to engineering units (i.e. any offset or factor used
 * by the signal are reversed) before being set into a 64-bit chunk at the
 * location determined by the bit field in the signal. All other bit fields in
 * the returend value will be 0, which may or may not interfere with other
 * recipients of the resulting CAN message.
 *
 * signal - The signal associated with the value
 * value - The numerical value to encode. String-ified state values need to be
 *      converted back to their integer equivalents before calling this
 *      function.
 *
 * Returns a 64-bit data block with the bit field for the signal set to the
 * encoded value.
 */
uint64_t encodeCanSignal(CanSignal* signal, float value);

/* Public: Encode and store a value in the bit field for a signal, but using the
 * given data as the starting point.
 *
 * This function is useful when you need to set more than one signal in a single
 * CAN message and you don't want them to clobber one another.
 *
 * signal - The signal associated with the value
 * value - The numerical value to encode. String-ified state values need to be
 *      converted back to their integer equivalents before calling this
 *      function.
 * data - the starting 64-bit data block to set the bit field in.
 *
 * Returns a the 64-bit data block provided but with the bit field for the
 * signal set to the encoded value.
 */
uint64_t encodeCanSignal(CanSignal* signal, float value, uint64_t data);

/* Public: Interpret the JSON value as a number and write it to the correct
 * bitfield for the given signal.
 *
 * signal - The signal associated with the value.
 * signals - An array of all CAN signals.
 * signalCount - The size of the CAN signals array.
 * value - The JSON object to write. The value will be interpreted as a double.
 * send - An output argument that will be set to false if the value should
 *     not be sent for any reason.
 *
 * Returns a 64-bit data block with the bit field for the signal set to the
 * encoded value.
 */
uint64_t numberWriter(CanSignal* signal, CanSignal* signals,
        int signalCount, cJSON* value, bool* send);

uint64_t numberWriter(CanSignal* signal, CanSignal* signals,
        int signalCount, cJSON* value, bool* send, uint64_t data);

/* Public: Convert the string value to the correct integer value for the given
 * CAN signal and write it to the signal's bitfield.
 *
 * signal - The signal associated with the value.
 * signals - An array of all CAN signals.
 * signalCount - The size of the CAN signals array.
 * value - The string object to write. The value should correspond to a signal
 *         state integer value.
 * send - An output argument that will be set to false if the value should
 *     not be sent for any reason.
 *
 * Returns a 64-bit data block with the bit field for the signal set to the
 * encoded value.
 */
uint64_t stateWriter(CanSignal* signal, CanSignal* signals,
        int signalCount, const char* value, bool* send);

/* Public: Interpret the JSON value as a string, then do the same as
 * stateWriter(CanSignal*, CanSignal*, int, const char*, bool*).
 */
uint64_t stateWriter(CanSignal* signal, CanSignal* signals,
        int signalCount, cJSON* value, bool* send);

uint64_t stateWriter(CanSignal* signal, CanSignal* signals,
        int signalCount, cJSON* value, bool* send, uint64_t data);

uint64_t stateWriter(CanSignal* signal, CanSignal* signals,
        int signalCount, const char* value, bool* send, uint64_t data);

/* Public: Interpret the JSON value as a boolean and write it to the correct
 * bitfield for the given signal. This will write either a 0 or 1.
 *
 * signal - The signal associated with the value.
 * signals - An array of all CAN signals.
 * signalCount - The size of the CAN signals array.
 * value - The JSON object to write. The value will be interpreted as a integer
 *      that represents a boolean.
 * send - An output argument that will be set to false if the value should
 *     not be sent for any reason.
 *
 * Returns a 64-bit data block with the bit field for the signal set to the
 * encoded value.
 */
uint64_t booleanWriter(CanSignal* signal, CanSignal* signals,
        int signalCount, cJSON* value, bool* send);

uint64_t booleanWriter(CanSignal* signal, CanSignal* signals,
        int signalCount, cJSON* value, bool* send, uint64_t data);

/* Public: Write a CAN signal with the given value to the bus.
 *
 * Using the provided CanSignal and writer function, convert the cJSON value
 * into a numerical value appropriate for the CAN signal. This may include
 * converting a string state value to its numerical equivalent, for example. The
 * writer function must know how to do this conversion (and return a fully
 * filled out uint64_t).
 *
 * signal - The CanSignal to send.
 * value - The value to send in the signal. This could be a boolean, number or
 *         string (i.e. a state value).
 * writer - A function to convert from the cJSON value to an encoded uint64_t.
 * signals - An array of all CAN signals.
 * signalCount - The size of the signals array.
 *
 * Returns true if the message was sent successfully.
 */
bool sendCanSignal(CanSignal* signal, cJSON* value,
        uint64_t (*writer)(CanSignal*, CanSignal*, int, cJSON*, bool*),
        CanSignal* signals, int signalCount);

/* Public: Write a CAN signal with the given value to the bus.
 *
 * Just like the above function sendCanSignal() that accepts a writer function,
 * but uses the CanSignal's value for "writeHandler" instead.
 *
 * See above for argument descriptions.
 */
bool sendCanSignal(CanSignal* signal, cJSON* value, CanSignal* signals,
        int signalCount);

/* Public: Send the given data on CAN using the signal's message ID and bus. The
 * data is assumed to be 64-bits and is used unmodified as the message's value.
 *
 * signal - the signal whose message we should write to CAN.
 * data - the data for the CAN message.
 * send - true if the message should actually be sent.
 *
 * Returns true if the message was sent on CAN.
 */
bool sendCanSignal(CanSignal* signal, uint64_t data, bool* send);

/* Public: The lowest-level API available to send a CAN message. The byte order
 * of the data is swapped, but otherwise this function queues the data to write
 * out to CAN without any additional processing.
 *
 * bus - the bus to send the message on.
 * messageId - the ID of the CAN message.
 * data - the data for the CAN message, byte order will be reversed.
 * send - true if the message should actually be sent.
 *
 * Returns true if the message was sent on CAN.
 */
bool enqueueCanMessage(CanBus* bus, uint32_t messageId, uint64_t data,
        bool* send);

/* Public: Write any queued outgoing messages to the CAN bus.
 *
 * bus - The CanBus instance that has a queued to be flushed out to CAN.
 */
void processCanWriteQueue(CanBus* bus);

/* Public: Write a CAN message with the given data and node ID to the bus.
 *
 * The CAN module has an 8 message buffer and sends messages in FIFO order. If
 * the buffer is full, this function will return false and the message will not
 * be sent.
 *
 * bus - The CAN bus to send the message on.
 * request - the CanMessage requested to send.
 *
 * Returns true if the message was sent successfully.
 */
bool sendCanMessage(CanBus* bus, CanMessage request);

#endif // _CANWRITE_H_
