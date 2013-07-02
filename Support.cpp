/**************************************************************** SUPPORT.CPP
 *                                                                          *
 *                Presentation Manager Support Functions                    *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include "Support.h"

 
/****************************************************************************
 *                                                                          *
 *      Check/Uncheck menu item.                                            *
 *                                                                          *
 ****************************************************************************/

extern void CheckMenuItem ( HWND Frame, USHORT MenuID, USHORT ItemID, BOOL Check ) {
   CheckMenuItem ( CHILD(Frame,MenuID), ItemID, Check ) ;
}

extern void CheckMenuItem ( HWND Menu, USHORT ItemID, BOOL Check ) {
   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(ItemID,TRUE), MPFROM2SHORT(MIA_CHECKED,Check?MIA_CHECKED:0) ) ;
}
 

