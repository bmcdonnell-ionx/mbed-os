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

#ifndef BYTE_BUFFER_H
#define BYTE_BUFFER_H

#include <cstdint>

class ByteBuffer {
public:

    /**
     * Create a byte buffer of the given size
     *
     * @param size Number of bytes this buffer can hold
     */
    ByteBuffer(std::uint32_t size=0);

    /**
     * Delete this byte buffer
     */
    ~ByteBuffer();

    /**
     * Set the size of the buffer
     *
     * Buffer contents are reset.
     *
     * @param size New buffer size
     */
    void resize(std::uint32_t size);

    /**
     * Add a single byte to this buffer
     *
     * There must be enough space in the buffer or the behavior is undefined.
     *
     * @param data byte to add
     */
    void push(std::uint8_t data);

    /**
     * Write a block of data to this ByteBuffer
     *
     * There must be enough space in the ByteBuffer or the behavior is undefined.
     *
     * @param data Block of data to write
     * @param size Size of data to write
     */
    void write(std::uint8_t *data, std::uint32_t size);

    /**
     * Remove a byte from this buffer
     *
     * @return data byte
     */
    std::uint8_t pop();

    /**
     * Read a block of data from this ByteBuffer into a buffer pointed by 'data'
     *
     * There must be enough data in the ByteBuffer or the behavior is undefined.
     *
     * @param data Block of data to read
     * @param size Size of data to read
     */
    void read(std::uint8_t *data, std::uint32_t size);

    /**
     * Return the number bytes in this byte buffer
     *
     * @return Number of used bytes
     */
    std::uint32_t size();

    /**
     * Return the number of additional bytes this buffer can hold
     *
     * @return Number of free bytes
     */
    std::uint32_t free();

    /**
     * Check if this byte buffer is full
     *
     * @return true if full, false otherwise
     */
    bool full();

    /**
     * Check if this byte buffer is empty
     *
     * @return true if empty, false otherwise
     */
    bool empty();

private:

    std::uint32_t _head;
    std::uint32_t _tail;
    std::uint32_t _size;
    std::uint8_t *_buf;
};


#endif
