/* 
 * $Id$
 *
 * :ts=4
 *
 * End markers for the CTOR and DTOR list.
 */

#if defined(__amigaos4__)

/****************************************************************************/

static void (*__CTOR_LIST__[1]) (void) __attribute__((used, section(".ctors"), aligned(sizeof(void (*)(void))) ));
static void (*__DTOR_LIST__[1]) (void) __attribute__((used, section(".dtors"), aligned(sizeof(void (*)(void))) ));

/****************************************************************************/

#endif /*__amigaos4__ */

