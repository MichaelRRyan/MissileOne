// Vector Calculations
// @Author Michael Rainsford Ryan
// C00239510
// @Date 16/11/2018
// Estimated time: 120m
// Session 1: 10:51 - 11:16 - 16/11/18
// Session 2: 21:46 - 23:56 - 19/11/18
// Time taken: 145m (Inconsistant work/many short, unlogged breaks)


#include "MyVector2.h"

/// <summary>
/// gets length of a vector using the formula: sqrt(Ux^2 + Uy^2)
/// </summary>
/// <param name="t_vector">input vector</param>
/// <returns>length</returns>
float vectorLength(sf::Vector2f t_vector)
{
	float sumOfSquares = (t_vector.x * t_vector.x ) + (t_vector.y * t_vector.y);
	const float length = sqrt(sumOfSquares);
	return length;
}

/// <summary>
/// gets length of a vector using the formula: Ux^2 + Uy^2
/// </summary>
/// <param name="t_vector">input vector</param>
/// <returns>length squared</returns>
float vectorLengthSquared(sf::Vector2f t_vector)
{
	const float lengthSquared = (t_vector.x * t_vector.x) + (t_vector.y * t_vector.y);
	return lengthSquared;
}

/// <summary>
/// Gets the cross product of two vectors using the formula: Ux * Vy - Uy * Vx
/// </summary>
/// <param name="t_vector">input vector A, input vector B</param>
/// <returns>cross product of two vectors</returns>
float vectorCrossProduct(sf::Vector2f t_vectorA, sf::Vector2f t_VectorB)
{
	const float crossProduct = (t_vectorA.x * t_VectorB.y) - (t_vectorA.y * t_VectorB.x);
	return crossProduct;
}

/// <summary>
/// Gets the dot product of two vectors using the formula: Ux * Vx + Uy * Vy
/// </summary>
/// <param name="t_vector">input vector A, input vector B</param>
/// <returns>dot product of two vectors</returns>
float vectorDotProduct(sf::Vector2f t_vectorA, sf::Vector2f t_VectorB)
{
	const float dotProduct = (t_vectorA.x * t_VectorB.x) + (t_vectorA.y * t_VectorB.y);
	return dotProduct;
}

/// <summary>
/// Gets the angle between two vectors using the formula: cos^-1(U.V / ||U|| * ||V||)
/// </summary>
/// <param name="t_vector">input vector A, input vector B</param>
/// <returns>angle between two vectors</returns>
float vectorAngleBetween(sf::Vector2f t_vectorA, sf::Vector2f t_VectorB)
{
	float magnitudesOfVectors = vectorLength(t_vectorA) * vectorLength(t_VectorB);

	float angleBetween = 0.0f;
	if (magnitudesOfVectors != 0) // Check for zero to avoid impossible division
	{
		float dotProductOverMagnitudes = vectorDotProduct(t_vectorA, t_VectorB) / magnitudesOfVectors;
		angleBetween = acosf(dotProductOverMagnitudes);
	}
	return angleBetween;
}

/// <summary>
/// Returns the inputted vector rotated by the inputted angle using the formula: Wx = Ux * cos(angle) - Uy * sin(angle), Wy = Ux * sin(angle) + Uy * cos(angle)
/// </summary>
/// <param name="t_vector">input vector, angle in radians</param>
/// <returns>rotated vector</returns>
sf::Vector2f vectorRotateBy(sf::Vector2f t_vector, float t_angleRadians)
{
	float rotatedX = t_vector.x * cosf(t_angleRadians) - t_vector.y * sinf(t_angleRadians);
	float rotatedY = t_vector.x * sinf(t_angleRadians) + t_vector.y * cosf(t_angleRadians);
	return sf::Vector2f(rotatedX, rotatedY);
}

/// <summary>
/// Gets the projection of a vector onto another using the formula: (U.V / ||V||) * (V / ||V||)
/// </summary>
/// <param name="t_vector">vector to project, vector to projected onto</param>
/// <returns>projected vector</returns>
sf::Vector2f vectorProjection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	sf::Vector2f projection{ 0.0f, 0.0f };
	if (vectorLength(t_onto) != 0)  // Check for zero to avoid impossible division
	{
		float dotProductByMagnitude = (vectorDotProduct(t_vector, t_onto) / vectorLength(t_onto));
		projection = dotProductByMagnitude * (t_onto / vectorLength(t_onto));
	}
	return projection;
}

/// <summary>
/// Gets the rejection of a vector projection using the formula: U - projV(U)
/// </summary>
/// <param name="t_vector">vector to project, vector to be projected onto</param>
/// <returns>rejected vector</returns>
sf::Vector2f vectorRejection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	const sf::Vector2f rejection = t_vector - vectorProjection(t_vector, t_onto);
	return rejection;
}

/// <summary>
/// Gets the scalar value of a projected vector using the formula: ||projV(U)||
/// </summary>
/// <param name="t_vector">vector to project, vector to be projected onto</param>
/// <returns>scalar projected vector</returns>
float vectorScalarProjection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	const float scalarProjection = vectorLength(vectorProjection(t_vector, t_onto));
	return scalarProjection;
}

/// <summary>
/// Gets the unit vector of a vector using the formula: U / ||U||
/// </summary>
/// <param name="t_vector">input vector</param>
/// <returns>unit vector</returns>
sf::Vector2f vectorUnitVector(sf::Vector2f t_vector)
{
	sf::Vector2f unitVector{ 0.0f, 0.0f };
	if (vectorLength(t_vector) != 0)  // Check for zero to avoid impossible division
	{
		unitVector = t_vector / vectorLength(t_vector);
	}
	return unitVector;
}
