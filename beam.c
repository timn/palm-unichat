/* $Id: beam.c,v 1.1.1.1 2003/02/06 20:15:57 tim Exp $
 *
 * Beam functions
 * Created: 2002-05-02
 */

#include "UniChat.h"
#include "beam.h"


/***********************************************************************
 * Send Functions
 ***********************************************************************/
static Err BeamBytes(ExgSocketPtr s, void *buffer, UInt32 bytesToSend) {
  Err err=0;

  while (!err && (bytesToSend >0)) {
    UInt32 bytesSent = ExgSend(s, buffer, bytesToSend, &err);
    bytesToSend -= bytesSent;
    buffer = ((Char *)buffer) + bytesSent;
  }

return err;
}


static Err BeamInit(ExgSocketType *s, Char *description) {
  MemSet(s, sizeof(ExgSocketType), 0);
  s->description=description;
  s->target=APP_CREATOR;
  // s.localMode=1;

  return ExgPut(s);
}


static Err BeamFinish(ExgSocketType *s) {
  Err err=errNone;
  return ExgDisconnect(s, err);
}


void BeamMessage(Char *message) {
  // We do not care about secret records, there shouldn't be any...
  Char description[16]="UniChat Message";
  ExgSocketType s;
  Err err;
  UInt8 beamType=BEAM_TYPE_MSG;
  UInt16 length=StrLen(message);

 
  err = BeamInit(&s, description);
  if (! err) {
    err = BeamBytes(&s, &beamType, sizeof(beamType));
    err = BeamBytes(&s, &length, sizeof(length));
    err = BeamBytes(&s, message, length);
  }
  err = BeamFinish(&s);
}


Err BeamReceive(ExgSocketPtr socketPtr, MemHandle m) {
  Err err=0;
  UInt8 beamType=0;
  UInt16 numBytes=0;

  err = ExgAccept(socketPtr);
  if (!err) {
    ExgReceive(socketPtr, &beamType, sizeof(beamType), &err);

    if (beamType == BEAM_TYPE_MSG) {
      if ((err == errNone) && (ExgReceive(socketPtr, &numBytes, sizeof(numBytes), &err) > 0)) {
        Char *buffer, *str;
        Char *curpos;
        UInt32 bytesReceived=0;
        UInt16 numBytesToRead=numBytes;
      
        MemHandleResize(m, numBytes+1);
        str=(Char *)MemHandleLock(m);
        MemSet(str, numBytes+1, 0);

        buffer=(char *)MemPtrNew(numBytes+1);
        curpos=str;

        // Receive the record
        do {
          bytesReceived = ExgReceive(socketPtr, buffer, numBytesToRead, &err);
          numBytes -= bytesReceived;

          MemMove(curpos, buffer, bytesReceived);
          curpos += bytesReceived;
        } while (!err && (bytesReceived > 0) && (numBytes > 0));
      
        MemPtrFree((MemPtr) buffer);
        MemHandleUnlock(m);
      }
    }
  }

return err;
}
