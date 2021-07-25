#include "Node.h"
#include "BTMacros.h"
#include "BTs.h"

template<typename T>
class FieldSetter : public LeafNode
{
public:
	inline SNode Clone(WBTs tree) override {
		auto cloneNode = std::dynamic_pointer_cast<FieldSetter<T>>(LeafNode::Clone(tree));
		cloneNode->_value = _value;
		cloneNode->_field = std::dynamic_pointer_cast<Field<T>>(tree.lock()->GetBlackBoard().lock()->GetField<T>(_field.lock()->GetFieldName()).lock());
		return cloneNode;
	}
	inline void CustomLoad(nlohmann::json& input) override {
		_value = input[BTField::valueField].get<T>();
		_field = _tree.lock()->GetBlackBoard().lock()->GetField<T>(input[BTField::fieldField].get<std::string>());
	}
	inline Node::State Tick() override {
		_field.lock()->SetValue(_value);
		return State::Success;
	}
protected:
	std::weak_ptr<Field<T>> _field;
	T _value = {};
};
#define DEFINE_SETTER_TYPE(nameForType, templateType) class nameForType : public FieldSetter<templateType> {NODE_REGISTER(nameForType);}

 DEFINE_SETTER_TYPE(StringSetter, std::string);
 DEFINE_SETTER_TYPE(CharSetter, char);
 DEFINE_SETTER_TYPE(IntSetter, int);
 DEFINE_SETTER_TYPE(UnsignedSetter, unsigned);
 DEFINE_SETTER_TYPE(FloatSetter, float);
 DEFINE_SETTER_TYPE(floatSetter, float);
 DEFINE_SETTER_TYPE(BoolSetter, bool);
 DEFINE_SETTER_TYPE(LongLongSetter, long long);
 DEFINE_SETTER_TYPE(UnsignedLongLongSetter, unsigned long long);