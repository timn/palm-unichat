/* $Id: UniChat.c,v 1.1 2003/02/06 20:15:57 tim Exp $
 *
 * UniChat.c: UniChat main() and event handling
 */

#include "UniChat.h"
#include "tnglue.h"
#include "gadget.h"
#include "beam.h"

/***********************************************************************
 * function is called at program start
 * you can put your own initialization jobs there
 ***********************************************************************/
static UInt16 StartApplication (void) {
  Err err = 0;

	// Initialize the random number seed;
	SysRandom( TimGetSeconds() );

  // Initialize TNglue
  err = TNGlueColorInit();

  GadgetInit();

  return err;
}

/***********************************************************************
 * function is checking ROM version of the palmos operating system
 * if ROM version is less then required the result of this function is
 * sysErrRomIncompatible
 ***********************************************************************/
static Err RomVersionCompatible (UInt32 requiredVersion, UInt16 launchFlags) {
	UInt32 romVersion;

	// See if we're on in minimum required version of the ROM or later.
	FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
	if (romVersion < requiredVersion) {
		if ((launchFlags & (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
  			(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) {
			FrmAlert (ALERT_ROMIncompatible);

			// PalmOS before 3.5 will continuously relaunch this app unless we switch to
			// another safe one.
			AppLaunchWithCommand(sysFileCDefaultApp, sysAppLaunchCmdNormalLaunch, NULL);
    }

    return (sysErrRomIncompatible);
  }

  return (0);
}



static void InputUpdateScrollBar(void) {
  UInt16 scrollPos;
  UInt16 textHeight;
  UInt16 fieldHeight;
  Int16 maxValue;
  FieldPtr fld;
  ScrollBarPtr bar;

  fld = TNGetObjectPtr(FIELD_input);
  bar = TNGetObjectPtr(SCROLLBAR_input);

  FldGetScrollValues (fld, &scrollPos, &textHeight,  &fieldHeight);

  if (textHeight > fieldHeight) {
  // On occasion, such as after deleting a multi-line selection of text,
  // the display might be the last few lines of a field followed by some
  // blank lines.  To keep the current position in place and allow the user
  // to "gracefully" scroll out of the blank area, the number of blank lines
  // visible needs to be added to max value.  Otherwise the scroll position
  // may be greater than maxValue, get pinned to maxvalue in SclSetScrollBar
  // resulting in the scroll bar and the display being out of sync.
  maxValue = (textHeight - fieldHeight) + FldGetNumberOfBlankLines (fld);
  } else if (scrollPos)  maxValue = scrollPos;
  else  maxValue = 0;

  SclSetScrollBar (bar, scrollPos, 0, maxValue, fieldHeight-1);
}


static void InputScroll(Int16 linesToScroll, Boolean updateScrollbar) {
  UInt16 blankLines;
  FieldPtr fld;

  fld = TNGetObjectPtr(FIELD_input);
  blankLines = FldGetNumberOfBlankLines (fld);

  if (linesToScroll < 0)  FldScrollField (fld, -linesToScroll, winUp);
  else if (linesToScroll > 0)  FldScrollField (fld, linesToScroll, winDown);

  // If there were blank lines visible at the end of the field
  // then we need to update the scroll bar.
  if (blankLines || updateScrollbar) {
    InputUpdateScrollBar();
  }
}



/***********************************************************************
 * function for main form initialization
 * you can put your initialization stuff there
 * e.g. initial settings for controls and labels
 ***********************************************************************/
static void MainFormInit (FormPtr frm){
  FieldType *fld;
  FieldAttrType attr;

  GadgetSet(frm, GADGET_main);
  FrmSetGadgetHandler(frm, FrmGetObjectIndex(frm, GADGET_main), GadgetHandler);

  fld=TNGetObjectPtr(FIELD_input);
  FldGetAttributes (fld, &attr);
  attr.hasScrollBar = true;
  FldSetAttributes (fld, &attr);
  FldSetScrollPosition (fld, 0);
  InputUpdateScrollBar();
}


/***********************************************************************
 * handling for the main drop down menu actions
 ***********************************************************************/
Boolean HandleMenuEvent (UInt16 command){
  Boolean handled = false;
  FieldType *fMsg;
  Char *msg;

  MenuEraseStatus(0);

  switch (command) {
    case MENUITEM_about:
      FrmDoDialog(FrmInitForm(FORM_about));
      handled=true;
      break;

    case MENUITEM_matrix:
			AppLaunchWithCommand(UNIMATRIX_APP_CREATOR, sysAppLaunchCmdNormalLaunch, NULL);
      handled=true;
      break;

    case MENUITEM_mensa:
			AppLaunchWithCommand(UNIMENSA_APP_CREATOR, sysAppLaunchCmdNormalLaunch, NULL);
      handled=true;
      break;

    case MENUITEM_send:
      fMsg=TNGetObjectPtr(FIELD_input);
      msg = FldGetTextPtr(fMsg);
      if (msg)  BeamMessage(FldGetTextPtr(fMsg));
      handled=true;
      break;


    default:
      break;
  }

  return handled;
}

Boolean HandleMenuOpenEvent(EventType *event) {
  DmSearchStateType searchState;
  UInt16 cardNo;
  LocalID dbID;
  Boolean needsSeparator=false;

  if (DmGetNextDatabaseByTypeCreator(true, &searchState, 0, UNIMENSA_APP_CREATOR, true, &cardNo, &dbID) != dmErrCantFind) {
    MenuAddItem(MENUITEM_send, MENUITEM_mensa, 'M', "UniMensa");
    needsSeparator=true;
  }
  if (DmGetNextDatabaseByTypeCreator(true, &searchState, 0, UNIMATRIX_APP_CREATOR, true, &cardNo, &dbID) != dmErrCantFind) {
    MenuAddItem(MENUITEM_send, MENUITEM_matrix, 'X', "UniMatrix");
    needsSeparator=true;
  }

  if (needsSeparator) {
    Char tmpSep[2];
    tmpSep[0]=MenuSeparatorChar;
    tmpSep[1]=0;
    MenuAddItem(MENUITEM_send, MENUITEM_addsep, 0, tmpSep);
  }  

  return true;
}


/***********************************************************************
 * handling for form and control actions
 * menu actions are forwarded to MainFormDoCommand
 ***********************************************************************/
static Boolean MainFormHandleEvent (EventPtr event){
  FormType *frm;
  Boolean handled = false;
  FieldType *fMsg;
  Char *msg;
  MemHandle m, old;
  
  if (event->eType == ctlSelectEvent) {
    // button handling
    handled = true;
    switch (event->data.ctlSelect.controlID) {
      // the ok button - this leaves the application

      case BUTTON_beam:
        fMsg=TNGetObjectPtr(FIELD_input);
        msg = FldGetTextPtr(fMsg);
        if (msg)  BeamMessage(FldGetTextPtr(fMsg));
        handled=true;
        break;

      case BUTTON_del:
        m = MemHandleNew(1);
        msg=(Char *)MemHandleLock(m);
        msg[0]=0;
        MemHandleUnlock(m);
        fMsg=TNGetObjectPtr(FIELD_input);
        old = FldGetTextHandle(fMsg);
        FldSetTextHandle(fMsg, m);
        if (old != NULL)  MemHandleFree(old); 
        FldDrawField(fMsg);
        FldSetScrollPosition(fMsg, 0);
        InputUpdateScrollBar();
        handled=true;
        break;

      case BUTTON_info:
        FrmDoDialog(FrmInitForm(FORM_about));
        handled=true;
        break;

      case BUTTON_help:
        FrmHelp(HELP_main);
        break;

      default:
        break;
      }
    } else if (event->eType == keyDownEvent) {
      // We have a hard button assigned and it was pressed
      if (TxtCharIsHardKey(event->data.keyDown.modifiers, event->data.keyDown.chr)) {
        if (! (event->data.keyDown.modifiers & poweredOnKeyMask)) {
          handled = true;
        }
      } else if (EvtKeydownIsVirtual(event)) {
        // Up or down keys pressed
        switch (event->data.keyDown.chr) {
          case vchrSendData:
            handled=true;
            fMsg=TNGetObjectPtr(FIELD_input);
            msg = FldGetTextPtr(fMsg);
            if (msg)  BeamMessage(FldGetTextPtr(fMsg));
            break;

          default:
            break;
        }
      }
    } else if (event->eType == sclRepeatEvent) {
      InputScroll(event->data.sclRepeat.newValue - event->data.sclRepeat.value, false);
      handled=false;
    } else if (event->eType == fldChangedEvent) {
      InputUpdateScrollBar ();
      handled = true;
    } else if (event->eType == menuOpenEvent) {
      return HandleMenuOpenEvent(event);
    } else if (event->eType == menuEvent) {
      // forwarding of menu events
      return HandleMenuEvent(event->data.menu.itemID);
    } else if (event->eType == frmUpdateEvent) {
      // redraws the form if needed
      FrmDrawForm (FrmGetActiveForm());
      handled = true;
    } else if (event->eType == frmOpenEvent) {
      // initializes and draws the form at program launch
      frm = FrmGetActiveForm();
      MainFormInit(frm);
      FrmDrawForm(frm);
      handled = true;
    } else if (event->eType == frmCloseEvent) {
      // this is done if program is closed
    }

  return (handled);
}


/***********************************************************************
 * chooses an displays the requested form
 ***********************************************************************/
static Boolean AppHandleEvent( EventPtr eventP) {
	UInt16 formId;
	FormPtr frmP;

	switch (eventP->eType) {
  	case frmLoadEvent:
      // Load the form resource.
		  formId = eventP->data.frmLoad.formID;
		  frmP = FrmInitForm(formId);
		  FrmSetActiveForm(frmP);

      // Set the event handler for the form.  The handler of the currently
      // active form is called by FrmHandleEvent each time is receives an
      // event.
		  switch (formId) {
			  case FORM_main:
				  FrmSetEventHandler(frmP, MainFormHandleEvent);
				  break;

  				ErrNonFatalDisplay("Invalid Form Load Event");
		  		break;
			}
		  break;

	  default:
		  return false;
	}
	return true;
}


/***********************************************************************
 * main event loop; loops until appStopEvent is caught or
 * QuitApp is set
 ***********************************************************************/
static void AppEventLoop(void){
	UInt16 error;
	EventType event;


	do {
		EvtGetEvent(&event, evtWaitForever);


		if (! SysHandleEvent(&event))
			if (! MenuHandleEvent(0, &event, &error))
				if (! AppHandleEvent(&event))
					FrmDispatchEvent(&event);

// Check the heaps after each event
		#if EMULATION_LEVEL != EMULATION_NONE
			MemHeapCheck(0);
			MemHeapCheck(1);
		#endif

	} while (event.eType != appStopEvent);

}

/***********************************************************************
 * application is finished, so we have to clean the desktop behind us
 ***********************************************************************/
static void StopApplication (void){
	FrmCloseAllForms ();
  GadgetFree();
//	CloseDatabase();
}


/***********************************************************************
 * main function
 ***********************************************************************/
UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags){
	UInt16 error = RomVersionCompatible (MINVERSION, launchFlags);
	if (error) return (error);

  /***
  * NORMAL STARTUP
  ****/
	if ( cmd == sysAppLaunchCmdNormalLaunch ) {
    error = StartApplication ();
    if (error) {
			// PalmOS before 3.5 will continuously relaunch this app unless we switch to
			// another safe one.
      FrmCustomAlert(ALERT_debug, "This BadBug(TM) should not happen...", "", "");
			AppLaunchWithCommand(sysFileCDefaultApp, sysAppLaunchCmdNormalLaunch, NULL);
      return error;
    }

    FrmGotoForm(FORM_main);

		AppEventLoop ();
		StopApplication ();

  /***
  * BEAMING
  ****/
	} else if (cmd == sysAppLaunchCmdSyncNotify) {
    // Register with the Exchange Manager
    // Not needed, we send by app creator
    // ExgRegisterData(APP_CREATOR, exgRegExtensionID, "uct");
  } else if (cmd == sysAppLaunchCmdExgAskUser) {
    // Always assume "Yes" as answer to the accept dialog since we display our
    // own on which the user can cancel the data
    ExgAskParamType *exgAskParam = (ExgAskParamType *)cmdPBP;
    exgAskParam->result=exgAskOk;
	} else if (cmd == sysAppLaunchCmdExgReceiveData) {
    // Is app active?
    if (launchFlags & sysAppLaunchFlagSubCall) {
      // Quit Forms
      FrmSaveAllForms();
      BeamReceive((ExgSocketPtr) cmdPBP, GadgetGetHandle());
      FrmGotoForm(FORM_main);
      
    } else {
      // Another app was running when we were called
      // We do not support that since we do not save the records anyway...
      FrmAlert(ALERT_notrunning);
    }
	}

	return 0;
}
