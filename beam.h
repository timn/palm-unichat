/* $Id: beam.h,v 1.1.1.1 2003/02/06 20:15:57 tim Exp $
 *
 * Beam functions
 * Created: 2002-05-02
 */

#define BEAM_DESC "UniMatrix Database"
#define BEAM_NAME "UniMatrix"
#define BEAM_ENDG ".umx"

#define BEAM_TYPE_MSG 1

extern void BeamMessage(Char *message);
extern Err BeamReceive(ExgSocketPtr socketPtr, MemHandle m);
