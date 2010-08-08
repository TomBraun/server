/*
Copyright (c) 1998-2010, Enno Rehling <enno@eressea.de>
                         Katja Zedel <katze@felidae.kn-bremen.de
                         Christian Schlittchen <corwin@amber.kn-bremen.de>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
**/

#ifndef H_KRNL_NAMES
#define H_KRNL_NAMES
#ifdef __cplusplus
extern "C" {
#endif
extern void register_names(void);
const char *undead_name(const struct unit * u);
const char *skeleton_name(const struct unit * u);
const char *zombie_name(const struct unit * u);
const char *ghoul_name(const struct unit * u);
const char *dragon_name(const struct unit *u);
const char *dracoid_name(const struct unit *u);
const char *generic_name(const struct unit *u);
const char *abkz(const char *s, char * buf, size_t size, size_t maxchars);

#ifdef __cplusplus
}
#endif
#endif
