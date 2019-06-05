#ifndef MYVARIABLE_H_INCLUDED
#define MYVARIABLE_H_INCLUDED

#include <assert.h>

#include "MyFlags.h"

//#include "MyStringSTL.h"
#include "MyArray.h"
#include "MyValue.h"

using namespace std;

enum VARIABLE_TYPE {
    IS_VALUE,
    IS_MATRIX,
    IS_ARRAY    //广义表
};

struct VARIABLE;

typedef MyArray<VARIABLE> ARRAY;

struct VARIABLE {
    VARIABLE_TYPE type;
    void* val;

    VARIABLE() {
        val = NULL;
    }
	VARIABLE(const VALUE& v) {
		type = IS_VALUE;
		val = new VALUE(v);
	}
    ~VARIABLE() {
        ;
    }
	void del() {
        if (!val) return;
        switch (type) {
        case IS_VALUE:
            delete (VALUE*)val;
            break;
        case IS_MATRIX:
            delete (MATRIX*)val;
            break;
		case IS_ARRAY:
			delete (ARRAY*)val;
			break;
        }
        val = NULL;
    }
    explicit operator bool() {
		assert(IS_VALUE == type);
        return (bool)*(VALUE*)val;
    }
    explicit operator size_t() {
        assert(IS_VALUE == type);
        return (size_t)*(VALUE*)val;
    }
    VARIABLE& Copy(const VARIABLE& rhs) {
        del();
        type = rhs.type;
        switch (type) {
        case IS_VALUE:
            val = new VALUE;
            ((VALUE*)val)->Copy(*(VALUE*)rhs.val);
            break;
        case IS_MATRIX:
            //val = new MATRIX;
            break;
		case IS_ARRAY:
			val = new ARRAY;
			*(ARRAY*)val = *(ARRAY*)rhs.val;
			break;
        }
        return *this;
    }
    VARIABLE operator + (const VARIABLE& rhs) const {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
        return VARIABLE(*(const VALUE*)val + *(const VALUE*)rhs.val);
    }
    VARIABLE operator - (const VARIABLE& rhs) const {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
        return VARIABLE(*(VALUE*)val - *(VALUE*)rhs.val);
    }
    VARIABLE operator * (const VARIABLE& rhs) const {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
        return VARIABLE(*(VALUE*)val * *(VALUE*)rhs.val);
    }
    VARIABLE operator / (const VARIABLE& rhs) const {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
        return VARIABLE(*(VALUE*)val / *(VALUE*)rhs.val);
    }
    VARIABLE operator % (const VARIABLE& rhs) const {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
        return VARIABLE(*(VALUE*)val % *(VALUE*)rhs.val);
    }

    VARIABLE operator += (const VARIABLE& rhs) {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
		*(VALUE*)val += *(const VALUE*)rhs.val;
		return *this;
    }
    VARIABLE operator -= (const VARIABLE& rhs) {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
		*(VALUE*)val -= *(const VALUE*)rhs.val;
		return *this;
    }
    VARIABLE operator *= (const VARIABLE& rhs) {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
		*(VALUE*)val *= *(const VALUE*)rhs.val;
		return *this;
    }
    VARIABLE operator /= (const VARIABLE& rhs) {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
		*(VALUE*)val /= *(const VALUE*)rhs.val;
		return *this;
    }
    VARIABLE operator %= (const VARIABLE& rhs) {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
		*(VALUE*)val %= *(const VALUE*)rhs.val;
		return *this;
    }

    VARIABLE operator < (const VARIABLE& rhs) const {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
        return VARIABLE(*(VALUE*)val < *(VALUE*)rhs.val);
    }
    VARIABLE operator > (const VARIABLE& rhs) const {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
        return VARIABLE(*(VALUE*)val > *(VALUE*)rhs.val);
    }
    VARIABLE operator <= (const VARIABLE& rhs) const {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
        return VARIABLE(*(VALUE*)val <= *(VALUE*)rhs.val);
    }
    VARIABLE operator >= (const VARIABLE& rhs) const {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
        return VARIABLE(*(VALUE*)val >= *(VALUE*)rhs.val);
    }
    VARIABLE operator == (const VARIABLE& rhs) const {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
        return VARIABLE(*(VALUE*)val == *(VALUE*)rhs.val);
    }
    VARIABLE operator != (const VARIABLE& rhs) const {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
        return VARIABLE(*(VALUE*)val != *(VALUE*)rhs.val);
    }
    VARIABLE operator && (const VARIABLE& rhs) const {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
        return VARIABLE(*(VALUE*)val && *(VALUE*)rhs.val);
    }
    VARIABLE operator || (const VARIABLE& rhs) const {
		assert(type == IS_VALUE && rhs.type == IS_VALUE);
        return VARIABLE(*(VALUE*)val || *(VALUE*)rhs.val);
    }

    void IfValueThenToArray() {
        if (type == IS_VALUE) {
            ARRAY* tmp = new ARRAY(1);
            (*tmp)(0) = *this;
            val = tmp;
            type = IS_ARRAY;
        }
    }
    VARIABLE& operator () (const vector<size_t>& pos) {
        IfValueThenToArray();
        return (*(ARRAY*)val)(pos);
    }
    bool OutOfBound(const vector<size_t>& pos) {
        IfValueThenToArray();
        return (*(ARRAY*)val).OutOfBound(pos);
    }
    void Print(ostream& out) const {
        switch (type) {
        case IS_VALUE:
            out << *(const VALUE*)val;
            break;
        case IS_MATRIX:
            out << *(const MATRIX*)val;
            break;
		case IS_ARRAY:
			out << *(const ARRAY*)val;
			break;
        }
    }
};

struct CONST_OR_VARIABLE{
    bool left_value;
    bool vari;
    VARIABLE* val;

    CONST_OR_VARIABLE() {
        val = NULL;
    }
    CONST_OR_VARIABLE(bool vari, bool lv) {
        Init(vari, lv);
    }
    /*~CONST_OR_VARIABLE()
    {
        del();
    }*/

    explicit operator bool() {
        return (bool)*val;
    }
    explicit operator size_t() {
        return (size_t)*val;
    }
    void del() {
        if (!left_value && val) {
            val->del();
        }
        val = NULL;
    }
    void Init(bool _vari, bool lv) {
        val = NULL;
        vari = _vari;
        left_value = lv;
    }
	void Init_new(bool _vari, bool lv) {
		vari = _vari;
		left_value = lv;
		val = new VARIABLE;
	}
    bool Copy(const CONST_OR_VARIABLE& rhs, ostream& info)
    {
        if(left_value && vari) {
            val->Copy(*rhs.val);
            return SUCCEED;
        } else {
            info << "Error: Assign to a right value or a constant value\n";
            return FAIL;
        }
    }
    CONST_OR_VARIABLE operator + (const CONST_OR_VARIABLE& rhs) const {
        static CONST_OR_VARIABLE ans;
		ans.Init_new(false, false);
        *ans.val = *val + *rhs.val;
        return ans;
    }
    CONST_OR_VARIABLE operator - (const CONST_OR_VARIABLE& rhs) const {
        static CONST_OR_VARIABLE ans;
		ans.Init_new(false, false);
        *ans.val = *val - *rhs.val;
        return ans;
    }
    CONST_OR_VARIABLE operator * (const CONST_OR_VARIABLE& rhs) const {
        static CONST_OR_VARIABLE ans;
		ans.Init_new(false, false);
        *ans.val = *val * *rhs.val;
        return ans;
    }
    CONST_OR_VARIABLE operator / (const CONST_OR_VARIABLE& rhs) const {
        static CONST_OR_VARIABLE ans;
		ans.Init_new(false, false);
        *ans.val = *val / *rhs.val;
        return ans;
    }
	CONST_OR_VARIABLE operator % (const CONST_OR_VARIABLE& rhs) const {
		static CONST_OR_VARIABLE ans;
		ans.Init_new(false, false);
		*ans.val = *val % *rhs.val;
		return ans;
	}

    CONST_OR_VARIABLE& operator += (const CONST_OR_VARIABLE& rhs) {
        assert(left_value && vari);
        *val += *rhs.val;
        return *this;
    }
    CONST_OR_VARIABLE& operator -= (const CONST_OR_VARIABLE& rhs) {
        assert(left_value && vari);
        *val -= *rhs.val;
        return *this;
    }
    CONST_OR_VARIABLE& operator *= (const CONST_OR_VARIABLE& rhs) {
        assert(left_value && vari);
        *val *= *rhs.val;
        return *this;
    }
    CONST_OR_VARIABLE& operator /= (const CONST_OR_VARIABLE& rhs) {
        assert(left_value && vari);
        *val /= *rhs.val;
        return *this;
    }
    CONST_OR_VARIABLE& operator %= (const CONST_OR_VARIABLE& rhs) {
        assert(left_value && vari);
        *val %= *rhs.val;
        return *this;
    }

	CONST_OR_VARIABLE operator < (const CONST_OR_VARIABLE& rhs) const {
		static CONST_OR_VARIABLE ans;
		ans.Init_new(false, false);
		*ans.val = *val < *rhs.val;
		return ans;
	}
	CONST_OR_VARIABLE operator <= (const CONST_OR_VARIABLE& rhs) const {
		static CONST_OR_VARIABLE ans;
		ans.Init_new(false, false);
		*ans.val = *val <= *rhs.val;
		return ans;
	}
	CONST_OR_VARIABLE operator > (const CONST_OR_VARIABLE& rhs) const {
		static CONST_OR_VARIABLE ans;
		ans.Init_new(false, false);
		*ans.val = *val > *rhs.val;
		return ans;
	}
	CONST_OR_VARIABLE operator >= (const CONST_OR_VARIABLE& rhs) const {
		static CONST_OR_VARIABLE ans;
		ans.Init_new(false, false);
		*ans.val = *val >= *rhs.val;
		return ans;
	}
    CONST_OR_VARIABLE operator == (const CONST_OR_VARIABLE& rhs) const {
		static CONST_OR_VARIABLE ans;
		ans.Init_new(false, false);
		*ans.val = *val == *rhs.val;
		return ans;
    }
    CONST_OR_VARIABLE operator != (const CONST_OR_VARIABLE& rhs) const {
		static CONST_OR_VARIABLE ans;
		ans.Init_new(false, false);
		*ans.val = *val == *rhs.val;
		return ans;
    }
    CONST_OR_VARIABLE operator && (const CONST_OR_VARIABLE& rhs) const {
		static CONST_OR_VARIABLE ans;
		ans.Init_new(false, false);
		*ans.val = *val && *rhs.val;
		return ans;
    }
    CONST_OR_VARIABLE operator || (const CONST_OR_VARIABLE& rhs) const {
		static CONST_OR_VARIABLE ans;
		ans.Init_new(false, false);
		*ans.val = *val || *rhs.val;
		return ans;
	}

    void Print(ostream& out)
    {
        val->Print(out);
    }
};

bool IsBeginningOfConst(char ch);
bool IsPartOfConst(char ch);

void GetConst(string& sth, const char*& str);
void GetConst(VARIABLE& val, const char* str);
void GetConst(VALUE& val, const char* str);
ostream& operator << (ostream& out, const VARIABLE& val);

#endif // MYVARIABLE_H_INCLUDED
