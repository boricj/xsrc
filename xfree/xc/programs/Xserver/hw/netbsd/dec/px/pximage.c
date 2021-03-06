/*	$NetBSD: pximage.c,v 1.2 2008/04/28 20:57:37 martin Exp $	*/

/*-
 * Copyright (c) 2001 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Andrew Doran.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/***********************************************************

Copyright (c) 1987  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.


Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of Digital not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
/* XConsortium: cfbimage.c,v 1.18 94/04/17 20:28:52 dpw Exp */

#include "px.h"

#include "X.h"
#include "pixmapstr.h"
#include "scrnintstr.h"
#include "cfb.h"
#include "cfbmskbits.h"
#include "servermd.h"
#include "mi.h"

#ifdef cfbCopyImagePlane
#undef cfbCopyImagePlane
#endif

/* XXX */
extern void cfbCopyImagePlane(
#if NeedFunctionPrototypes
    DrawablePtr /*pSrcDrawable*/,
    DrawablePtr /*pDstDrawable*/,
    int /*rop*/,
    RegionPtr /*prgnDst*/,
    DDXPointPtr /*pptSrc*/,
    unsigned long /*planemask*/
#endif
);

void
pxGetImage(pDrawable, sx, sy, w, h, format, planeMask, pdstLine)
    DrawablePtr pDrawable;
    int		sx, sy, w, h;
    unsigned int format;
    unsigned long planeMask;
    char	*pdstLine;
{
    pxScreenPrivPtr sp;
    BoxRec box;
    DDXPointRec ptSrc;
    RegionRec rgnDst;
    ScreenPtr pScreen;
    PixmapPtr pPixmap;

    if ((w == 0) || (h == 0))
	return;

    if (pDrawable->type == DRAWABLE_WINDOW) {
	miGetImage (pDrawable, sx, sy, w, h, format, planeMask, pdstLine);
	return;
    }

    if (pDrawable->bitsPerPixel == 1)
    {
    	/* Drawable won't be on-screen. */
	mfbGetImage(pDrawable, sx, sy, w, h, format, planeMask, pdstLine);
	return;
    }
    pScreen = pDrawable->pScreen;
    if (format == ZPixmap)
    {
	pPixmap = GetScratchPixmapHeader(pScreen, w, h, 
			pDrawable->depth, pDrawable->bitsPerPixel,
			PixmapBytePad(w,pDrawable->depth), (pointer)pdstLine);
	if (!pPixmap)
	    return;
	if ((planeMask & PMSK) != PMSK)
	    bzero((char *)pdstLine, pPixmap->devKind * h);
        ptSrc.x = sx + pDrawable->x;
        ptSrc.y = sy + pDrawable->y;
        box.x1 = 0;
        box.y1 = 0;
        box.x2 = w;
        box.y2 = h;
        REGION_INIT(pScreen, &rgnDst, &box, 1);
	cfbDoBitblt(pDrawable, (DrawablePtr)pPixmap, GXcopy, &rgnDst,
		    &ptSrc, planeMask);
        REGION_UNINIT(pScreen, &rgnDst);
	FreeScratchPixmapHeader(pPixmap);
    }
    else
    {
	sp = pDrawable->pScreen->devPrivates[pxScreenPrivateIndex].ptr;

	if (sp->bpp == 8) {
	    pPixmap = GetScratchPixmapHeader(pScreen, w, h, /*depth*/ 1,
			/*bpp*/ 1, BitmapBytePad(w), (pointer)pdstLine);
	    if (!pPixmap)
	        return;

            ptSrc.x = sx + pDrawable->x;
            ptSrc.y = sy + pDrawable->y;
            box.x1 = 0;
            box.y1 = 0;
            box.x2 = w;
            box.y2 = h;
            REGION_INIT(pScreen, &rgnDst, &box, 1);
	    cfbCopyImagePlane (pDrawable, (DrawablePtr)pPixmap, GXcopy,
	        &rgnDst, &ptSrc, planeMask);
            REGION_UNINIT(pScreen, &rgnDst);
	    FreeScratchPixmapHeader(pPixmap);
	} else
	    miGetImage (pDrawable, sx, sy, w, h, format, planeMask, pdstLine);
    }
}
