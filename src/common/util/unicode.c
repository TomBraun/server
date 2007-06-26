/* vi: set ts=2:
 * +-------------------+  Christian Schlittchen <corwin@amber.kn-bremen.de>
 * |                   |  Enno Rehling <enno@eressea.de>
 * | Eressea PBEM host |  Katja Zedel <katze@felidae.kn-bremen.de>
 * | (c) 1998 - 2007   |  
 * |                   |  This program may not be used, modified or distributed
 * +-------------------+  without prior permission by the authors of Eressea.
 *  
 */

#include <config.h>
#include "unicode.h"

#include <errno.h>

/* Convert a UTF-8 encoded character to UCS-4. */
int
unicode_utf8_to_ucs4(wint_t *ucs4_character, const xmlChar *utf8_string, 
                     size_t *length)
{
  xmlChar utf8_character = utf8_string[0];

  /* Is the character in the ASCII range? If so, just copy it to the
  output. */
  if (utf8_character <= 0x7F)
  {
    *ucs4_character = utf8_character;
    *length = 1;
  }
  else if ((utf8_character & 0xE0) == 0xC0)
  {
    /* A two-byte UTF-8 sequence. Make sure the other byte is good. */
    if (utf8_string[1] != '\0' &&
      (utf8_string[1] & 0xC0) != 0x80)
    {
      return EILSEQ;
    }

    *ucs4_character =
      ((utf8_string[1] & 0x3F) << 0) +
      ((utf8_character & 0x1F) << 6);
    *length = 2;
  }
  else if ((utf8_character & 0xF0) == 0xE0)
  {
    /* A three-byte UTF-8 sequence. Make sure the other bytes are
    good. */
    if ((utf8_string[1] != '\0') &&
      (utf8_string[1] & 0xC0) != 0x80 &&
      (utf8_string[2] != '\0') &&
      (utf8_string[2] & 0xC0) != 0x80)
    {
      return EILSEQ;
    }

    *ucs4_character = 
      ((utf8_string[2] & 0x3F) << 0) +
      ((utf8_string[1] & 0x3F) << 6) +
      ((utf8_character & 0x0F) << 12);
    *length = 3;
  }
  else if ((utf8_character & 0xF8) == 0xF0)
  {
    /* A four-byte UTF-8 sequence. Make sure the other bytes are
    good. */
    if ((utf8_string[1] != '\0') &&
      (utf8_string[1] & 0xC0) != 0x80 &&
      (utf8_string[2] != '\0') &&
      (utf8_string[2] & 0xC0) != 0x80 &&
      (utf8_string[3] != '\0') &&
      (utf8_string[3] & 0xC0) != 0x80)
    {
      return EILSEQ;
    }

    *ucs4_character = 
      ((utf8_string[3] & 0x3F) << 0) +
      ((utf8_string[2] & 0x3F) << 6) +
      ((utf8_string[1] & 0x3F) << 12) +
      ((utf8_character & 0x07) << 18);
    *length = 4;
  }
  else if ((utf8_character & 0xFC) == 0xF8)
  {
    /* A five-byte UTF-8 sequence. Make sure the other bytes are
    good. */
    if ((utf8_string[1] != '\0') &&
      (utf8_string[1] & 0xC0) != 0x80 &&
      (utf8_string[2] != '\0') &&
      (utf8_string[2] & 0xC0) != 0x80 &&
      (utf8_string[3] != '\0') &&
      (utf8_string[3] & 0xC0) != 0x80 &&
      (utf8_string[4] != '\0') &&
      (utf8_string[4] & 0xC0) != 0x80)
    {
      return EILSEQ;
    }

    *ucs4_character = 
      ((utf8_string[4] & 0x3F) << 0) +
      ((utf8_string[3] & 0x3F) << 6) +
      ((utf8_string[2] & 0x3F) << 12) +
      ((utf8_string[1] & 0x3F) << 18) +
      ((utf8_character & 0x03) << 24);
    *length = 5;
  }
  else if ((utf8_character & 0xFE) == 0xFC)
  {
    /* A six-byte UTF-8 sequence. Make sure the other bytes are
    good. */
    if ((utf8_string[1] != '\0') &&
      (utf8_string[1] & 0xC0) != 0x80 &&
      (utf8_string[2] != '\0') &&
      (utf8_string[2] & 0xC0) != 0x80 &&
      (utf8_string[3] != '\0') &&
      (utf8_string[3] & 0xC0) != 0x80 &&
      (utf8_string[4] != '\0') &&
      (utf8_string[4] & 0xC0) != 0x80 &&
      (utf8_string[5] != '\0') &&
      (utf8_string[5] & 0xC0) != 0x80)
    {
      return EILSEQ;
    }

    *ucs4_character = 
      ((utf8_string[5] & 0x3F) << 0) +
      ((utf8_string[4] & 0x3F) << 6) +
      ((utf8_string[3] & 0x3F) << 12) +
      ((utf8_string[2] & 0x3F) << 18) +
      ((utf8_string[1] & 0x3F) << 24) +
      ((utf8_character & 0x01) << 30);
    *length = 6;
  }
  else
  {
    return EILSEQ;
  }

  return 0;
}
