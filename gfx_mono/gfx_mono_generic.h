/**
 * \file
 *
 * \brief Generic monochrome LCD graphic primitives
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#ifndef GFX_MONO_GENERIC
#define GFX_MONO_GENERIC

#include <stdint.h>
#include "../rgb_led.h"

#ifdef __cplusplus
extern "C" {
#endif
      
    typedef uint8_t gfx_coord_t;

    void PutPixel(uint8_t x, uint8_t y);
    
    /**
     * \ingroup asfdoc_common2_gfx_mono
     * \defgroup asfdoc_common2_gfx_mono_generic_group Generic monochrome graphic primitives
     *
     * This is a service providing generic implementations of graphic primitives
     * - Horizontal line
     * - Vertical line
     * - Line
     * - Circle (filled/not filled)
     * - Rectangle (filled/not filled)
     *
     * it also provides functionality to draw a bitmap to the graphic memory.
     *
     * These functions are made available if the graphic hardware being used do
     * not implement the functionality in hardware. This is true in most cases.
     *
     * This service is included as a requirement for a
     * hardware specific component that uses these functions, and provides a
     * asfdoc_common2_draw_pixel function.
     *
     * @{
     */

    void gfx_mono_draw_horizontal_line(gfx_coord_t x, gfx_coord_t y,
            gfx_coord_t length);

    void gfx_mono_draw_vertical_line(gfx_coord_t x, gfx_coord_t y,
            gfx_coord_t length);

    void gfx_mono_draw_line(gfx_coord_t x1, gfx_coord_t y1,
            gfx_coord_t x2, gfx_coord_t y2);

    void gfx_mono_draw_rect(gfx_coord_t x, gfx_coord_t y,
            gfx_coord_t width, gfx_coord_t height);

    void gfx_mono_draw_filled_rect(gfx_coord_t x, gfx_coord_t y,
            gfx_coord_t width, gfx_coord_t height);

    void gfx_mono_draw_circle(gfx_coord_t x, gfx_coord_t y,
            gfx_coord_t radius, uint8_t octant_mask);

    void gfx_mono_draw_filled_circle(gfx_coord_t x, gfx_coord_t y,
            gfx_coord_t radius, uint8_t quadrant_mask);

    void gfx_mono_put_bitmap(uint8_t *bitmap, gfx_coord_t x,
            gfx_coord_t y);

    void gfx_mono_draw_pixel(gfx_coord_t x, gfx_coord_t y);

    void gfx_mono_draw_char(char c, uint8_t x, uint8_t y);

    void gfx_mono_draw_string(const char* str, uint8_t x, uint8_t y);

    /** @} */



    /** Bitmask for drawing circle octant 0. */
#define GFX_OCTANT0     (1 << 0)
    /** Bitmask for drawing circle octant 1. */
#define GFX_OCTANT1     (1 << 1)
    /** Bitmask for drawing circle octant 2. */
#define GFX_OCTANT2     (1 << 2)
    /** Bitmask for drawing circle octant 3. */
#define GFX_OCTANT3     (1 << 3)
    /** Bitmask for drawing circle octant 4. */
#define GFX_OCTANT4     (1 << 4)
    /** Bitmask for drawing circle octant 5. */
#define GFX_OCTANT5     (1 << 5)
    /** Bitmask for drawing circle octant 6. */
#define GFX_OCTANT6     (1 << 6)
    /** Bitmask for drawing circle octant 7. */
#define GFX_OCTANT7     (1 << 7)

    /** Bitmask for drawing circle quadrant 0. */
#define GFX_QUADRANT0   (GFX_OCTANT0 | GFX_OCTANT1)
    /** Bitmask for drawing circle quadrant 1. */
#define GFX_QUADRANT1   (GFX_OCTANT2 | GFX_OCTANT3)
    /** Bitmask for drawing circle quadrant 2. */
#define GFX_QUADRANT2   (GFX_OCTANT4 | GFX_OCTANT5)
    /** Bitmask for drawing circle quadrant 3. */
#define GFX_QUADRANT3   (GFX_OCTANT6 | GFX_OCTANT7)

    /** Bitmask for drawing left half of circle. */
#define GFX_LEFTHALF    (GFX_QUADRANT3 | GFX_QUADRANT0)
    /** Bitmask for drawing top half of circle. */
#define GFX_TOPHALF     (GFX_QUADRANT0 | GFX_QUADRANT1)
    /** Bitmask for drawing right half of circle. */
#define GFX_RIGHTHALF   (GFX_QUADRANT1 | GFX_QUADRANT2)
    /** Bitmask for drawing bottom half of circle. */
#define GFX_BOTTOMHALF  (GFX_QUADRANT2 | GFX_QUADRANT3)

    /** Bitmask for drawing whole circle. */
#define GFX_WHOLE       0xFF


#ifdef __cplusplus
}
#endif

#endif /* GFX_MONO_GENERIC */
