// Menus.h

#define MAX_MENU_DEPTH 23


struct Menus_in_System  {
    // This info gets pushed onto the menu stack 
    // with a call to "Start_a_Menu(WhichMenu)".  (WBG)
    // Handles sub-menus up to 23 deep.
    void *MenuPtr;
    int16 LengthOfMenu;
} ;

struct a_menu { // each menu consists of the text to
    // display, a function pointer, and the param
    // value to send to the function. The function
    // is executed when the user clicks the 'enter' button.
    char MenuItem[21]; // text to display.
    void *Service;  // function pointer
    int16 param;    // the parameter to send to the function.
};

struct menu_stack {
    // This get pushed onto the menu stack.
    // Handles sub-menus up to 20 deep.
    void *MenuPtrOnStack;
    int16 LCD_PointerOnStack;
    int16 LengthOfMenuOnStack;
} ;


extern struct Menus_in_System The_Menus_in_System[];
extern struct a_menu IndividualTests_Menu_List[];
extern struct a_menu Main_MenuList[];
extern struct a_menu MyMenuList[];
extern struct a_menu *MenuPtr;
extern struct menu_stack The_menu_stack[];

extern int16 TheMenuLength;
extern int16 LCD_Upd_Dpy;

extern int16 Display_SW_Version(int16 New_LCD_Menu);
extern int16 QueryMenuVariables(int16 dummy);
extern int16 QueryMenuVariables(int16 dummy);
extern int16 PushMenuPtrArray(void *MenuPtr);