/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * core/gmr_core_alloc.h
 * Copyright (C) 2016 Dr.NP <np@bsgroup.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name ``Dr.NP'' nor the name of any other
 *    contributor may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * libgmr IS PROVIDED BY Dr.NP ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL Dr.NP OR ANY OTHER CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file core/gmr_core_alloc.h
 * @author Dr.NP <np@bsgroup.org>
 * @date 11/07/2017
 * @update -/-/-
 * @version 0.0.1
 *
 * This file is part of libgmr project.
 * Memory allocator and manager
 */

#ifndef __GMR_CORE_ALLOC_H

#define __GMR_CORE_ALLOC_H
// Memory allocation operations
#define gmr_malloc(gmr, size)           gmr_alloc_real(gmr, NULL, size, GMR_FALSE)
#define gmr_calloc(gmr, nmemb, size)    gmr_alloc_real(gmr, NULL, (nmemb * size), GMR_TRUE)
#define gmr_realloc(gmr, ptr, size)     gmr_alloc_real(gmr, ptr, size, GMR_FALSE)
#define gmr_crealloc(gmr, ptr, size)    gmr_alloc_real(gmr, ptr, size, GMR_TRUE)
#define gmr_free(gmr, ptr)              gmr_alloc_real(gmr, ptr, 0, GMR_FALSE)

GMR_DECLARE(gmr_ptr) gmr_alloc_real(GMR *gmr, gmr_ptr old_ptr, gmr_size new_size, gmr_boolean zero_fill);

#endif  /* __GMR_CORE_ALLOC_H */
