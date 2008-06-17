#include <iostream>

#include <sipstack/UnknownParameter.hxx>
#include <sipstack/HeaderFieldValue.hxx>
#include <sipstack/ParserCategory.hxx>
#include <sipstack/Symbols.hxx>

using namespace std;
using namespace Vocal2;

HeaderFieldValue::HeaderFieldValue()
   : next(0),
     mParserCategory(0),
     mField(0),
     mFieldLength(0),
     mMine(false)
{}

HeaderFieldValue::HeaderFieldValue(const char* field, unsigned int fieldLength)
  : next(0),
    mParserCategory(0),
    mField(field),
    mFieldLength(fieldLength),
    mMine(false)
{}

HeaderFieldValue::HeaderFieldValue(const HeaderFieldValue& hfv)
  : next(hfv.next),
    mParserCategory(hfv.mParserCategory->clone(this)),
    mField(0),
    mFieldLength(hfv.mFieldLength),
    mParameterList(hfv.mParameterList),
    mUnknownParameterList(hfv.mUnknownParameterList),
    mMine(true)
{
  // if this isn't parsed, chunk and copy the block of memory
  // the copy for the param lists will end up with empty lists
  if (!(isParsed()))
    {
      const_cast<char*&>(mField) = new char[mFieldLength];
      memcpy(const_cast<char*>(mField), hfv.mField, mFieldLength);
    }
  
  // if it is, the above will end up with null unparsed fields and valid 
  // param lists
}

HeaderFieldValue::~HeaderFieldValue()
{
  if (mMine)
    {
      HeaderFieldValue* ncThis = const_cast<HeaderFieldValue*>(this);      
      delete [] ncThis->mField;
    }
}

// make destructor

HeaderFieldValue* 
HeaderFieldValue::clone() const
{
  return new HeaderFieldValue(*this);
}
      
void 
HeaderFieldValue::parseParameters(const char* startPos)
{
   const char* endPos = mField + mFieldLength;
   if (startPos < endPos)
   {
      assert(*startPos == Symbols::SEMI_COLON[0]);
      //const char* endKey = scan(startPos, endPos, Symbols::SEMI_OR_EQUAL);
      
      
   }
}

#if 0
const char* 
HeaderFieldValue::scan(const char* start, const char* end, const Data& charset)
{
}
#endif

ParameterList& 
HeaderFieldValue::getParameters()
{
  return mParameterList;
}

ParameterList& 
HeaderFieldValue::getUnknownParameters()
{
  return mParameterList;
}

bool 
HeaderFieldValue::isParsed() const
{
  return mParserCategory != 0;
}


bool 
HeaderFieldValue::exists(const Data& subcomponent)
{
  
  Parameter* exists = mUnknownParameterList.find(subcomponent);
  if (!exists)
  {
     exists = mParameterList.find(subcomponent);
     if (exists)
     {
        throw ParseException("???", __FILE__, __LINE__); // !jf!
     }
  }
  return exists;
}


bool 
HeaderFieldValue::exists(const ParameterTypes::Type type)
{
  
  return mParameterList.find(type);
  
}
void 
HeaderFieldValue::remove(const ParameterTypes::Type type)
{
   mParameterList.erase(type);
}

void 
HeaderFieldValue::remove(const Data& parameter)
{
   mUnknownParameterList.erase(parameter);
}

UnknownParameter* 
HeaderFieldValue::get(const Data& type)
{
  return dynamic_cast<UnknownParameter*>(mUnknownParameterList.get(type));
}

ostream& 
HeaderFieldValue::encode(ostream& str) const
{
   if (mParserCategory != 0 && mParserCategory->isParsed())
   {
      return mParserCategory->encode(str);
   }
   str.write(mField, mFieldLength);
   str << endl;
   return str;
}

ostream& Vocal2::operator<<(ostream& stream, HeaderFieldValue& hfv)
{
   if (hfv.isParsed())
   {
      hfv.mParameterList.encode(stream);
      stream << Symbols::SPACE << Symbols::COLON << Symbols::SPACE;
      hfv.mUnknownParameterList.encode(stream);
   }
   else
   {
      stream << Data(hfv.mField, hfv.mFieldLength);
   }
   return stream;
}


