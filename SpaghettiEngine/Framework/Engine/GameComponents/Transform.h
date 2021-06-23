#pragma once
#include "Matrix.h"
#include "Vector3.h"
#include "Macros.h"
#include "CornException.h"

#include <deque>
#include <memory>

CLASS_FORWARD_DECLARATION(Transform);

class Transform
{
	friend class ChildContainer;
public:
	class TransformException : public CornException
	{
	public:
		TransformException(int line, const char* file, const std::wstring& des) noexcept;
		virtual const wchar_t* What() const noexcept override;
		virtual const wchar_t* GetType() const noexcept override;
	protected:
		std::wstring _description;
	};
public:
#pragma region Getters
	[[nodiscard]] PTransform		GetParent() const;

	[[nodiscard]] Matrix4			GetWorldMatrix();
	[[nodiscard]] Vector3			GetWorldTransform() const;
	[[nodiscard]] Vector3			GetWorldRotation() const;
	[[nodiscard]] Vector3			GetWorldScale() const;
	[[nodiscard]] Vector3			GetTransform() const;
	[[nodiscard]] Vector3			GetRotation() const;
	[[nodiscard]] Vector3			GetScale() const;
#pragma endregion

#pragma region Setters
	void SetRotation(const float& x, const float& y, const float& z);
	void SetScale(const float& x, const float& y, const float& z);
	void SetTransform(const float& x, const float& y, const float& z);
	void SetRotation(const Vector3& vec3);
	void SetScale(const Vector3& vec3);
	void SetTransform(const Vector3& vec3);
	void Rotate(const float& x, const float& y, const float& z);
	void Rotate(const Vector3& rotation);

	void Move(const Vector3& vector);
	void Move(const float& x, const float& y, const float& z);

	void Translate(const Vector3& vector);
	void Translate(const float& x, const float& y, const float& z);
#pragma endregion
protected:
	void SetParent(PTransform parent);
	void AddChild(PTransform child);
	void RemoveChild(PTransform child);

	void ForceRecalculateMatrix();

	void CalculateRotationMatrix();
	void CalculateTransformMatrix();
	void CalculateScaleMatrix();
	void CalculateWorldMatrix();
protected:
	PTransform _parentTransform;
	std::deque<PTransform> _childTransform;

	// Use for know when to update what cause it's expensive;
	bool _isTransformChanged = true;
	bool _isRotationChanged = true;
	bool _isScaleChanged = true;
	bool _isChanged = true;

	// Use for storing transformations;
	Matrix4 _transformMatrix;
	Matrix4 _rotationMatrix;
	Matrix4 _scaleMatrix;
	Matrix4 _worldMatrix;

	// Raw values
	Vector3 _transform;
	Vector3 _rotation; // In degree
	Vector3 _scale;
};

