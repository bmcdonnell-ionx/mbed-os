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

#ifndef USBPHYHW_H
#define USBPHYHW_H

#include "USBPhy.h"


class USBPhyHw : public USBPhy {
public:
    USBPhyHw();
    virtual ~USBPhyHw();
    virtual void init(USBPhyEvents *events);
    virtual void deinit();
    virtual bool powered();
    virtual void connect();
    virtual void disconnect();
    virtual void configure();
    virtual void unconfigure();
    virtual void sof_enable();
    virtual void sof_disable();
    virtual void set_address(std::uint8_t address);
    virtual void remote_wakeup();
    virtual const usb_ep_table_t* endpoint_table();

    virtual std::uint32_t ep0_set_max_packet(std::uint32_t max_packet);
    virtual void ep0_setup_read_result(std::uint8_t *buffer, std::uint32_t size);
    virtual void ep0_read(std::uint8_t *data, std::uint32_t size);
    virtual std::uint32_t ep0_read_result();
    virtual void ep0_write(std::uint8_t *buffer, std::uint32_t size);
    virtual void ep0_stall();

    virtual bool endpoint_add(usb_ep_t endpoint, std::uint32_t max_packet, usb_ep_type_t type);
    virtual void endpoint_remove(usb_ep_t endpoint);
    virtual void endpoint_stall(usb_ep_t endpoint);
    virtual void endpoint_unstall(usb_ep_t endpoint);

    virtual bool endpoint_read(usb_ep_t endpoint, std::uint8_t *data, std::uint32_t size);
    virtual std::uint32_t endpoint_read_result(usb_ep_t endpoint);
    virtual bool endpoint_write(usb_ep_t endpoint, std::uint8_t *data, std::uint32_t size);
    virtual void endpoint_abort(usb_ep_t endpoint);

    virtual void process();

private:
    USBPhyEvents *events;
    std::uint8_t *read_buffers[16];
    std::uint16_t read_sizes[16];

    bool endpoint_read_core(usb_ep_t endpoint, std::uint32_t max_packet);
    bool endpoint_read_result_core(usb_ep_t endpoint, std::uint8_t *data, std::uint32_t size, std::uint32_t *bytesRead);


    static void _usbisr(void);
};

#endif
