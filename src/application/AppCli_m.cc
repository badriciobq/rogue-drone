//
// Generated file, do not edit! Created by nedtool 4.6 from application/AppCli.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "AppCli_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

Register_Class(EtherAppPct);

EtherAppPct::EtherAppPct(const char *name, int kind) : ::cPacket(name,kind)
{
    this->key_var = 0;
    this->nodeId_var = 0;
    this->macAddress_var = 0;
}

EtherAppPct::EtherAppPct(const EtherAppPct& other) : ::cPacket(other)
{
    copy(other);
}

EtherAppPct::~EtherAppPct()
{
}

EtherAppPct& EtherAppPct::operator=(const EtherAppPct& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void EtherAppPct::copy(const EtherAppPct& other)
{
    this->key_var = other.key_var;
    this->nodeId_var = other.nodeId_var;
    this->macAddress_var = other.macAddress_var;
}

void EtherAppPct::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->key_var);
    doPacking(b,this->nodeId_var);
    doPacking(b,this->macAddress_var);
}

void EtherAppPct::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->key_var);
    doUnpacking(b,this->nodeId_var);
    doUnpacking(b,this->macAddress_var);
}

const char * EtherAppPct::getKey() const
{
    return key_var.c_str();
}

void EtherAppPct::setKey(const char * key)
{
    this->key_var = key;
}

int EtherAppPct::getNodeId() const
{
    return nodeId_var;
}

void EtherAppPct::setNodeId(int nodeId)
{
    this->nodeId_var = nodeId;
}

const char * EtherAppPct::getMacAddress() const
{
    return macAddress_var.c_str();
}

void EtherAppPct::setMacAddress(const char * macAddress)
{
    this->macAddress_var = macAddress;
}

class EtherAppPctDescriptor : public cClassDescriptor
{
  public:
    EtherAppPctDescriptor();
    virtual ~EtherAppPctDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(EtherAppPctDescriptor);

EtherAppPctDescriptor::EtherAppPctDescriptor() : cClassDescriptor("EtherAppPct", "cPacket")
{
}

EtherAppPctDescriptor::~EtherAppPctDescriptor()
{
}

bool EtherAppPctDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<EtherAppPct *>(obj)!=NULL;
}

const char *EtherAppPctDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int EtherAppPctDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int EtherAppPctDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *EtherAppPctDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "key",
        "nodeId",
        "macAddress",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int EtherAppPctDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='k' && strcmp(fieldName, "key")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "nodeId")==0) return base+1;
    if (fieldName[0]=='m' && strcmp(fieldName, "macAddress")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *EtherAppPctDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "int",
        "string",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *EtherAppPctDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int EtherAppPctDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    EtherAppPct *pp = (EtherAppPct *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string EtherAppPctDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    EtherAppPct *pp = (EtherAppPct *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getKey());
        case 1: return long2string(pp->getNodeId());
        case 2: return oppstring2string(pp->getMacAddress());
        default: return "";
    }
}

bool EtherAppPctDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    EtherAppPct *pp = (EtherAppPct *)object; (void)pp;
    switch (field) {
        case 0: pp->setKey((value)); return true;
        case 1: pp->setNodeId(string2long(value)); return true;
        case 2: pp->setMacAddress((value)); return true;
        default: return false;
    }
}

const char *EtherAppPctDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *EtherAppPctDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    EtherAppPct *pp = (EtherAppPct *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


