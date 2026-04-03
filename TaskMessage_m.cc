//
// Generated file, do not edit! Created by opp_msgtool 6.3 from TaskMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "TaskMessage_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(TaskMessage)

TaskMessage::TaskMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

TaskMessage::TaskMessage(const TaskMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

TaskMessage::~TaskMessage()
{
    delete [] this->arrayChunk;
}

TaskMessage& TaskMessage::operator=(const TaskMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void TaskMessage::copy(const TaskMessage& other)
{
    this->sourceId = other.sourceId;
    this->targetId = other.targetId;
    this->subtaskId = other.subtaskId;
    delete [] this->arrayChunk;
    this->arrayChunk = (other.arrayChunk_arraysize==0) ? nullptr : new int[other.arrayChunk_arraysize];
    arrayChunk_arraysize = other.arrayChunk_arraysize;
    for (size_t i = 0; i < arrayChunk_arraysize; i++) {
        this->arrayChunk[i] = other.arrayChunk[i];
    }
    this->isResult_ = other.isResult_;
    this->maxResult = other.maxResult;
}

void TaskMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->sourceId);
    doParsimPacking(b,this->targetId);
    doParsimPacking(b,this->subtaskId);
    b->pack(arrayChunk_arraysize);
    doParsimArrayPacking(b,this->arrayChunk,arrayChunk_arraysize);
    doParsimPacking(b,this->isResult_);
    doParsimPacking(b,this->maxResult);
}

void TaskMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->sourceId);
    doParsimUnpacking(b,this->targetId);
    doParsimUnpacking(b,this->subtaskId);
    delete [] this->arrayChunk;
    b->unpack(arrayChunk_arraysize);
    if (arrayChunk_arraysize == 0) {
        this->arrayChunk = nullptr;
    } else {
        this->arrayChunk = new int[arrayChunk_arraysize];
        doParsimArrayUnpacking(b,this->arrayChunk,arrayChunk_arraysize);
    }
    doParsimUnpacking(b,this->isResult_);
    doParsimUnpacking(b,this->maxResult);
}

int TaskMessage::getSourceId() const
{
    return this->sourceId;
}

void TaskMessage::setSourceId(int sourceId)
{
    this->sourceId = sourceId;
}

int TaskMessage::getTargetId() const
{
    return this->targetId;
}

void TaskMessage::setTargetId(int targetId)
{
    this->targetId = targetId;
}

int TaskMessage::getSubtaskId() const
{
    return this->subtaskId;
}

void TaskMessage::setSubtaskId(int subtaskId)
{
    this->subtaskId = subtaskId;
}

size_t TaskMessage::getArrayChunkArraySize() const
{
    return arrayChunk_arraysize;
}

int TaskMessage::getArrayChunk(size_t k) const
{
    if (k >= arrayChunk_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)arrayChunk_arraysize, (unsigned long)k);
    return this->arrayChunk[k];
}

void TaskMessage::setArrayChunkArraySize(size_t newSize)
{
    int *arrayChunk2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = arrayChunk_arraysize < newSize ? arrayChunk_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        arrayChunk2[i] = this->arrayChunk[i];
    for (size_t i = minSize; i < newSize; i++)
        arrayChunk2[i] = 0;
    delete [] this->arrayChunk;
    this->arrayChunk = arrayChunk2;
    arrayChunk_arraysize = newSize;
}

void TaskMessage::setArrayChunk(size_t k, int arrayChunk)
{
    if (k >= arrayChunk_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)arrayChunk_arraysize, (unsigned long)k);
    this->arrayChunk[k] = arrayChunk;
}

void TaskMessage::insertArrayChunk(size_t k, int arrayChunk)
{
    if (k > arrayChunk_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)arrayChunk_arraysize, (unsigned long)k);
    size_t newSize = arrayChunk_arraysize + 1;
    int *arrayChunk2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        arrayChunk2[i] = this->arrayChunk[i];
    arrayChunk2[k] = arrayChunk;
    for (i = k + 1; i < newSize; i++)
        arrayChunk2[i] = this->arrayChunk[i-1];
    delete [] this->arrayChunk;
    this->arrayChunk = arrayChunk2;
    arrayChunk_arraysize = newSize;
}

void TaskMessage::appendArrayChunk(int arrayChunk)
{
    insertArrayChunk(arrayChunk_arraysize, arrayChunk);
}

void TaskMessage::eraseArrayChunk(size_t k)
{
    if (k >= arrayChunk_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)arrayChunk_arraysize, (unsigned long)k);
    size_t newSize = arrayChunk_arraysize - 1;
    int *arrayChunk2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        arrayChunk2[i] = this->arrayChunk[i];
    for (i = k; i < newSize; i++)
        arrayChunk2[i] = this->arrayChunk[i+1];
    delete [] this->arrayChunk;
    this->arrayChunk = arrayChunk2;
    arrayChunk_arraysize = newSize;
}

bool TaskMessage::isResult() const
{
    return this->isResult_;
}

void TaskMessage::setIsResult(bool isResult)
{
    this->isResult_ = isResult;
}

int TaskMessage::getMaxResult() const
{
    return this->maxResult;
}

void TaskMessage::setMaxResult(int maxResult)
{
    this->maxResult = maxResult;
}

class TaskMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_sourceId,
        FIELD_targetId,
        FIELD_subtaskId,
        FIELD_arrayChunk,
        FIELD_isResult,
        FIELD_maxResult,
    };
  public:
    TaskMessageDescriptor();
    virtual ~TaskMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(TaskMessageDescriptor)

TaskMessageDescriptor::TaskMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(TaskMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

TaskMessageDescriptor::~TaskMessageDescriptor()
{
    delete[] propertyNames;
}

bool TaskMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<TaskMessage *>(obj)!=nullptr;
}

const char **TaskMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *TaskMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int TaskMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 6+base->getFieldCount() : 6;
}

unsigned int TaskMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_sourceId
        FD_ISEDITABLE,    // FIELD_targetId
        FD_ISEDITABLE,    // FIELD_subtaskId
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_arrayChunk
        FD_ISEDITABLE,    // FIELD_isResult
        FD_ISEDITABLE,    // FIELD_maxResult
    };
    return (field >= 0 && field < 6) ? fieldTypeFlags[field] : 0;
}

const char *TaskMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "sourceId",
        "targetId",
        "subtaskId",
        "arrayChunk",
        "isResult",
        "maxResult",
    };
    return (field >= 0 && field < 6) ? fieldNames[field] : nullptr;
}

int TaskMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "sourceId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "targetId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "subtaskId") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "arrayChunk") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "isResult") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "maxResult") == 0) return baseIndex + 5;
    return base ? base->findField(fieldName) : -1;
}

const char *TaskMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_sourceId
        "int",    // FIELD_targetId
        "int",    // FIELD_subtaskId
        "int",    // FIELD_arrayChunk
        "bool",    // FIELD_isResult
        "int",    // FIELD_maxResult
    };
    return (field >= 0 && field < 6) ? fieldTypeStrings[field] : nullptr;
}

const char **TaskMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *TaskMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int TaskMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    TaskMessage *pp = omnetpp::fromAnyPtr<TaskMessage>(object); (void)pp;
    switch (field) {
        case FIELD_arrayChunk: return pp->getArrayChunkArraySize();
        default: return 0;
    }
}

void TaskMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    TaskMessage *pp = omnetpp::fromAnyPtr<TaskMessage>(object); (void)pp;
    switch (field) {
        case FIELD_arrayChunk: pp->setArrayChunkArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'TaskMessage'", field);
    }
}

const char *TaskMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    TaskMessage *pp = omnetpp::fromAnyPtr<TaskMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string TaskMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    TaskMessage *pp = omnetpp::fromAnyPtr<TaskMessage>(object); (void)pp;
    switch (field) {
        case FIELD_sourceId: return long2string(pp->getSourceId());
        case FIELD_targetId: return long2string(pp->getTargetId());
        case FIELD_subtaskId: return long2string(pp->getSubtaskId());
        case FIELD_arrayChunk: return long2string(pp->getArrayChunk(i));
        case FIELD_isResult: return bool2string(pp->isResult());
        case FIELD_maxResult: return long2string(pp->getMaxResult());
        default: return "";
    }
}

void TaskMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    TaskMessage *pp = omnetpp::fromAnyPtr<TaskMessage>(object); (void)pp;
    switch (field) {
        case FIELD_sourceId: pp->setSourceId(string2long(value)); break;
        case FIELD_targetId: pp->setTargetId(string2long(value)); break;
        case FIELD_subtaskId: pp->setSubtaskId(string2long(value)); break;
        case FIELD_arrayChunk: pp->setArrayChunk(i,string2long(value)); break;
        case FIELD_isResult: pp->setIsResult(string2bool(value)); break;
        case FIELD_maxResult: pp->setMaxResult(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TaskMessage'", field);
    }
}

omnetpp::cValue TaskMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    TaskMessage *pp = omnetpp::fromAnyPtr<TaskMessage>(object); (void)pp;
    switch (field) {
        case FIELD_sourceId: return pp->getSourceId();
        case FIELD_targetId: return pp->getTargetId();
        case FIELD_subtaskId: return pp->getSubtaskId();
        case FIELD_arrayChunk: return pp->getArrayChunk(i);
        case FIELD_isResult: return pp->isResult();
        case FIELD_maxResult: return pp->getMaxResult();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'TaskMessage' as cValue -- field index out of range?", field);
    }
}

void TaskMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    TaskMessage *pp = omnetpp::fromAnyPtr<TaskMessage>(object); (void)pp;
    switch (field) {
        case FIELD_sourceId: pp->setSourceId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_targetId: pp->setTargetId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_subtaskId: pp->setSubtaskId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_arrayChunk: pp->setArrayChunk(i,omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_isResult: pp->setIsResult(value.boolValue()); break;
        case FIELD_maxResult: pp->setMaxResult(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TaskMessage'", field);
    }
}

const char *TaskMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr TaskMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    TaskMessage *pp = omnetpp::fromAnyPtr<TaskMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void TaskMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    TaskMessage *pp = omnetpp::fromAnyPtr<TaskMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TaskMessage'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

