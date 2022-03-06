#include <BWUtils/Exceptions.h>

namespace buwai {

SIMPLE_DEFINE_EXCEPTION(IOException, ExceptionBase);

SIMPLE_DEFINE_EXCEPTION(UnsupportedAndroidSDKException, ExceptionBase);

SIMPLE_DEFINE_EXCEPTION(NullPointerException, ExceptionBase);

SIMPLE_DEFINE_EXCEPTION(InvalidValueException, ExceptionBase);

SIMPLE_DEFINE_EXCEPTION(RemoteException, ExceptionBase);

SIMPLE_DEFINE_EXCEPTION(ArrayIndexOutOfBoundsException, ExceptionBase);

SIMPLE_DEFINE_EXCEPTION(DynamicLinkException, ExceptionBase);

SIMPLE_DEFINE_EXCEPTION(StringException, ExceptionBase);

SIMPLE_DEFINE_EXCEPTION(MemoryAllocationFailureException, ExceptionBase);

SIMPLE_DEFINE_EXCEPTION(NoSuchMethodException, ExceptionBase);

}
