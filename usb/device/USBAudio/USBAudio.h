/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef USBAudio_H
#define USBAudio_H

/* These headers are included for child class. */
#include "USBDescriptor.h"
#include "USBDevice_Types.h"

#include "USBDevice.h"
#include "Callback.h"
#include "OperationList.h"
#include "ByteBuffer.h"
#include "rtos/EventFlags.h"

/**
* USBAudio example
*
* @code
* #include "mbed.h"
* #include "USBAudio.h"
*
* // Audio loopback example use:
* // 1. Select "Mbed Audio" as your sound device
* // 2. Play a song or audio file
* // 3. Record the output using a program such as Audacity
*
* int main() {
*
*     USBAudio audio(true, 44100, 2, 44100, 2);
*
*     printf("Looping audio\r\n");
*     static std::uint8_t buf[128];
*     while (true) {
*         if (!audio.read(buf, sizeof(buf))) {
*             memset(buf, 0, sizeof(buf));
*         }
*         audio.write(buf, sizeof(buf));
*     }
* }
* @endcode
*/
class USBAudio: protected USBDevice {
public:

    enum AudioEvent {
        Start,
        Transfer,
        End
    };

    /**
    * Basic constructor
    *
    * Construct this object optionally connecting.
    *
    * @note Do not use this constructor in derived classes.
    *
    * @param connect Call connect on initialization
    * @param frequency_rx frequency in Hz (default: 48000)
    * @param channel_count_rx channel number (1 or 2) (default: 1)
    * @param frequency_tx frequency in Hz (default: 8000)
    * @param channel_count_tx channel number (1 or 2) (default: 1)
    * @param buffer_ms time audio can be buffered without overflowing in milliseconds
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your product_release
    */
    USBAudio(bool connect=true, std::uint32_t frequency_rx = 48000, std::uint8_t channel_count_rx = 1, std::uint32_t frequency_tx = 8000, std::uint8_t channel_count_tx = 1, std::uint32_t buffer_ms=10, std::uint16_t vendor_id = 0x7bb8, std::uint16_t product_id = 0x1111, std::uint16_t product_release = 0x0100);

    /**
    * Fully featured constructor
    *
    * Construct this object with the supplied USBPhy and parameters. The user
    * this object is responsible for calling connect() or init().
    *
    * @note Derived classes must use this constructor and call init() or
    * connect() themselves. Derived classes should also call deinit() in
    * their destructor. This ensures that no interrupts can occur when the
    * object is partially constructed or destroyed.
    *
    * @param phy USB phy to use
    * @param frequency_rx frequency in Hz (default: 48000)
    * @param channel_count_rx channel number (1 or 2) (default: 1)
    * @param frequency_tx frequency in Hz (default: 8000)
    * @param channel_count_tx channel number (1 or 2) (default: 1)
    * @param buffer_ms time audio can be buffered without overflowing in milliseconds
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your product_release
    */
    USBAudio(USBPhy *phy, std::uint32_t frequency_rx, std::uint8_t channel_count_rx, std::uint32_t frequency_tx, std::uint8_t channel_count_tx, std::uint32_t buffer_ms, std::uint16_t vendor_id, std::uint16_t product_id, std::uint16_t product_release);

    /**
     * Destroy this object
     *
     * Any classes which inherit from this class must call deinit
     * before this destructor runs.
     */
    virtual ~USBAudio();

    /**
    * Connect USBAudio
    */
    void connect();

    /**
    * Disconnect USBAudio
    *
    * This unblocks all calls to read_ready and write_ready.
    */
    void disconnect();

    /**
    * Read audio data
    *
    * @param buf pointer on a buffer which will be filled with audio data
    * @param size size to read
    *
    * @returns true if successful
    */
    bool read(std::uint8_t *buf, std::uint32_t size);

    /**
    * Nonblocking audio data read
    *
    * Read the available audio data.
    *
    * @param buf pointer on a buffer which will be filled with audio data
    * @param size size to read
    * @param actual size actually read
    * @note This function is safe to call from USBAudio callbacks.
    */
    void read_nb(std::uint8_t *buf, std::uint32_t size, std::uint32_t *actual);

    /**
     * Return the number read packets dropped due to overflow
     *
     * @param clear Reset the overflow count back to 0
     * @return Number of packets dropped due to overflow
     */
    std::uint32_t read_overflows(bool clear=false);

    /**
     * Check if the audio read channel is open
     *
     * @return true if the audio read channel open, false otherwise
     */
    bool read_ready();

    /**
     * Wait until the audio read channel is open
     */
    void read_wait_ready();

    /**
    * Write audio data
    *
    * @param buf pointer to audio data to write
    * @param size size to write
    *
    * @returns true if successful
    */
    bool write(std::uint8_t *buf, std::uint32_t size);

    /**
    * Nonblocking audio data write
    *
    * Write the available audio data.
    *
    * @param buf pointer to audio data to write
    * @param size size to write
    * @param actual actual size written
    * @note This function is safe to call from USBAudio callbacks.
    */
    void write_nb(std::uint8_t *buf, std::uint32_t size, std::uint32_t *actual);

    /**
     * Return the number write packets not sent due to underflow
     *
     * @param clear Reset the underflow count back to 0
     * @return Number of packets that should have been
     *         sent but weren't due to overflow
     */
    std::uint32_t write_underflows(bool clear=false);

    /**
     * Check if the audio write channel is open
     *
     * @return true if the audio write channel open, false otherwise
     */
    bool write_ready();

    /**
     * Wait until the audio write channel is open
     */
    void write_wait_ready();

    /**
    * Get current volume between 0.0 and 1.0
    *
    * @returns volume
    */
    float get_volume();

    /** Attach a Callback to update the volume
     *
     * @param cb Callback to attach
     *
     */
    void attach(mbed::Callback<void()> &cb);

    /** attach a Callback to Tx Done
     *
     * @param cb Callback to attach
     *
     */
    void attach_tx(mbed::Callback<void(AudioEvent)> &cb);

    /** attach a Callback to Rx Done
     *
     * @param cb Callback to attach
     *
     */
    void attach_rx(mbed::Callback<void(AudioEvent)> &cb);

protected:

    virtual void callback_state_change(DeviceState new_state);
    virtual void callback_request(const setup_packet_t *setup);
    virtual void callback_request_xfer_done(const setup_packet_t *setup, bool aborted);
    virtual void callback_set_configuration(std::uint8_t configuration);
    virtual void callback_set_interface(std::uint16_t interface, std::uint8_t alternate);

    virtual const std::uint8_t *string_iproduct_desc();
    virtual const std::uint8_t *string_iinterface_desc();
    virtual const std::uint8_t *configuration_desc(std::uint8_t index);

private:

    class AsyncWrite;
    class AsyncRead;

    enum ChannelState {
        Powerdown,
        Closed,
        Opened
    };

    void _init(std::uint32_t frequency_rx, std::uint8_t channel_count_rx, std::uint32_t frequency_tx, std::uint8_t channel_count_tx, std::uint32_t buffer_ms);

    /*
    * Call to rebuild the configuration descriptor
    *
    * This function should be called on creation or when any
    * value that is part of the configuration descriptor
    * changes.
    * @note This function uses ~200 bytes of stack so
    * make sure your stack is big enough for it.
    */
    void _build_configuration_desc();

    void _receive_change(ChannelState new_state);
    void _receive_isr();
    void _send_change(ChannelState new_state);
    void _send_isr_start();
    void _send_isr_next_sync();
    void _send_isr();

    // has connect been called
    bool _connected;

    // audio volume
    float _volume;

    // mute state
    std::uint8_t _mute;

    // Volume Current Value
    std::uint16_t _vol_cur;

    // Volume Minimum Value
    std::uint16_t _vol_min;

    // Volume Maximum Value
    std::uint16_t _vol_max;

    // Volume Resolution
    std::uint16_t _vol_res;

    // callback to update volume
    mbed::Callback<void()> _update_vol;

    // callback transmit Done
    mbed::Callback<void(AudioEvent)> _tx_done;

    // callback receive Done
    mbed::Callback<void(AudioEvent)> _rx_done;

    // Number of times data was dropped due to an overflow
    std::uint32_t _rx_overflow;

    // Number of times data was not sent due to an underflow
    std::uint32_t _tx_underflow;

    // frequency in Hz
    std::uint32_t _tx_freq;
    std::uint32_t _rx_freq;

    // mono, stereo,...
    std::uint8_t _rx_channel_count;
    std::uint8_t _tx_channel_count;

    bool _tx_idle;
    std::uint16_t _tx_frame_fract;
    std::uint16_t _tx_whole_frames_per_xfer;
    std::uint16_t _tx_fract_frames_per_xfer;

    // size of the maximum packet for the isochronous endpoint
    std::uint16_t _tx_packet_size_max;
    std::uint16_t _rx_packet_size_max;

    // Buffer used for the isochronous transfer
    std::uint8_t *_tx_packet_buf;
    std::uint8_t *_rx_packet_buf;

    // Holding buffer
    ByteBuffer _tx_queue;
    ByteBuffer _rx_queue;

    // State of the audio channels
    ChannelState _tx_state;
    ChannelState _rx_state;


    // sample - a single PCM audio sample
    // frame    - a group of samples from each channel
    // packet   - a group of frames sent over USB in one transfer

    // Blocking primitives
    OperationList<AsyncWrite> _write_list;
    OperationList<AsyncRead> _read_list;
    rtos::EventFlags _flags;

    // endpoint numbers
    usb_ep_t _episo_out;    // rx endpoint
    usb_ep_t _episo_in;     // tx endpoint

    // channel config in the configuration descriptor: master, left, right
    std::uint8_t _channel_config_rx;
    std::uint8_t _channel_config_tx;

    // configuration descriptor
    std::uint8_t _config_descriptor[183];

    // buffer for control requests
    std::uint8_t _control_receive[2];

};

#endif
