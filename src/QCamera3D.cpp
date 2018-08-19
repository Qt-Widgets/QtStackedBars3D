/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Data Visualization module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "QCamera3DPrivate.h"
#include "QUtils3D.h"

#include <QtCore/qmath.h>

namespace QtStackedBar3DVis
{

	/*!
	* \class QCamera3D
	* \inmodule QtDataVisualization
	* \brief Representation of a camera in 3D space.
	* \since QtDataVisualization 1.0
	*
	* QCamera3D represents a basic orbit around centerpoint 3D camera that is used when rendering the
	* data visualization. The class offers simple methods for rotating the camera around the origin
	* and setting zoom level.
	*/

	/*!
	* \enum QCamera3D::CameraPreset
	*
	* Predefined positions for camera.
	*
	* \value CameraPresetNone
	*        Used to indicate a preset has not been set, or the scene has been rotated freely.
	* \value CameraPresetFrontLow
	* \value CameraPresetFront
	* \value CameraPresetFrontHigh
	* \value CameraPresetLeftLow
	* \value CameraPresetLeft
	* \value CameraPresetLeftHigh
	* \value CameraPresetRightLow
	* \value CameraPresetRight
	* \value CameraPresetRightHigh
	* \value CameraPresetBehindLow
	* \value CameraPresetBehind
	* \value CameraPresetBehindHigh
	* \value CameraPresetIsometricLeft
	* \value CameraPresetIsometricLeftHigh
	* \value CameraPresetIsometricRight
	* \value CameraPresetIsometricRightHigh
	* \value CameraPresetDirectlyAbove
	* \value CameraPresetDirectlyAboveCW45
	* \value CameraPresetDirectlyAboveCCW45
	* \value CameraPresetFrontBelow
	*        In QBars3D from CameraPresetFrontBelow onward these only work for graphs including negative
	*        values. They act as Preset...Low for positive-only values.
	* \value CameraPresetLeftBelow
	* \value CameraPresetRightBelow
	* \value CameraPresetBehindBelow
	* \value CameraPresetDirectlyBelow
	*        Acts as CameraPresetFrontLow for positive-only bars.
	*/

	/*!
	* \qmltype Camera3D
	* \inqmlmodule QtDataVisualization
	* \since QtDataVisualization 1.0
	* \ingroup datavisualization_qml
	* \instantiates QCamera3D
	* \brief Representation of a camera in 3D space.
	*
	* Camera3D represents a basic orbit around centerpoint 3D camera that is used when rendering the
	* data visualization. The type offers simple methods for rotating the camera around the origin
	* and setting zoom level.
	*
	* For Camera3D enums, see \l{QCamera3D::CameraPreset}.
	*/

	/*!
	* \qmlproperty float Camera3D::xRotation
	*
	* The X-rotation angle of the camera around the target point in degrees
	* starting from the current base position.
	*/

	/*!
	* \qmlproperty float Camera3D::yRotation
	*
	* The Y-rotation angle of the camera around the target point in degrees
	* starting from the current base position.
	*/

	/*!
	* \qmlproperty Camera3D.CameraPreset Camera3D::cameraPreset
	*
	* The currently active camera preset, which is one of
	* \l{QCamera3D::CameraPreset}{Camera3D.CameraPreset}. If no preset is active, the value
	* is \l{QCamera3D::CameraPresetNone}{Camera3D.CameraPresetNone}.
	*/

	/*!
	* \qmlproperty float Camera3D::zoomLevel
	*
	* The camera zoom level in percentage. The default value of \c{100.0}
	* means there is no zoom in or out set in the camera.
	* The value is limited by the minZoomLevel and maxZoomLevel properties.
	*
	* \sa minZoomLevel, maxZoomLevel
	*/

	/*!
	* \qmlproperty float Camera3D::minZoomLevel
	*
	* Sets the minimum allowed camera zoom level.
	* If the new minimum level is higher than the existing maximum level, the maximum level is
	* adjusted to the new minimum as well.
	* If the current zoomLevel is outside the new bounds, it is adjusted as well.
	* The minZoomLevel cannot be set below \c{1.0}.
	* Defaults to \c{10.0}.
	*
	* \sa zoomLevel, maxZoomLevel
	*/

	/*!
	* \qmlproperty float Camera3D::maxZoomLevel
	*
	* Sets the maximum allowed camera zoom level.
	* If the new maximum level is lower than the existing minimum level, the minimum level is
	* adjusted to the new maximum as well.
	* If the current zoomLevel is outside the new bounds, it is adjusted as well.
	* Defaults to \c{500.0f}.
	*
	* \sa zoomLevel, minZoomLevel
	*/

	/*!
	* \qmlproperty bool Camera3D::wrapXRotation
	*
	* The behavior of the minimum and maximum limits in the X-rotation.
	* By default, the X-rotation wraps from minimum value to maximum and from
	* maximum to minimum.
	*
	* If set to \c true, the X-rotation of the camera is wrapped from minimum to
	* maximum and from maximum to minimum. If set to \c false, the X-rotation of
	* the camera is limited to the sector determined by the minimum and maximum
	* values.
	*/

	/*!
	* \qmlproperty bool Camera3D::wrapYRotation
	*
	* The behavior of the minimum and maximum limits in the Y-rotation.
	* By default, the Y-rotation is limited between the minimum and maximum values
	* without any wrapping.
	*
	* If \c true, the Y-rotation of the camera is wrapped from minimum to maximum
	* and from maximum to minimum. If \c false, the Y-rotation of the camera is
	* limited to the sector determined by the minimum and maximum values.
	*/

	/*!
	* \qmlproperty vector3d Camera3D::target
	* \since QtDataVisualization 1.2
	*
	* The camera target as a vector3d. Defaults to \c {vector3d(0.0, 0.0, 0.0)}.
	*
	* Valid coordinate values are between \c{-1.0...1.0}, where the edge values indicate
	* the edges of the corresponding axis range. Any values outside this range are clamped to the edge.
	*
	* \note For bar graphs, the Y-coordinate is ignored and camera always targets a point on
	* the horizontal background.
	*/

	/*!
	* Constructs a new 3D camera with position set to origin, up direction facing towards the Y-axis
	* and looking at origin by default. An optional \a parent parameter can be given and is then passed
	* to QObject constructor.
	*/
	QCamera3D::QCamera3D(QObject *parent) :
		QObject3D(parent),
		d_ptr(new QCamera3DPrivate(this))
	{
	}

	/*!
	*  Destroys the camera object.
	*/
	QCamera3D::~QCamera3D()
	{
	}

	/*!
	* Copies the 3D camera's properties from the given source camera.
	* Values are copied from the \a source to this object.
	*/
	void QCamera3D::copyValuesFrom(const QObject3D &source)
	{
		// Note: Do not copy values from parent, as we are handling the position internally

		const QCamera3D &sourceCamera = static_cast<const QCamera3D &>(source);

		d_ptr->m_requestedTarget = sourceCamera.d_ptr->m_requestedTarget;

		d_ptr->m_xRotation = sourceCamera.d_ptr->m_xRotation;
		d_ptr->m_yRotation = sourceCamera.d_ptr->m_yRotation;

		d_ptr->m_minXRotation = sourceCamera.d_ptr->m_minXRotation;
		d_ptr->m_minYRotation = sourceCamera.d_ptr->m_minYRotation;
		d_ptr->m_maxXRotation = sourceCamera.d_ptr->m_maxXRotation;
		d_ptr->m_maxYRotation = sourceCamera.d_ptr->m_maxYRotation;

		d_ptr->m_wrapXRotation = sourceCamera.d_ptr->m_wrapXRotation;
		d_ptr->m_wrapYRotation = sourceCamera.d_ptr->m_wrapYRotation;

		d_ptr->m_zoomLevel = sourceCamera.d_ptr->m_zoomLevel;
		d_ptr->m_minZoomLevel = sourceCamera.d_ptr->m_minZoomLevel;
		d_ptr->m_maxZoomLevel = sourceCamera.d_ptr->m_maxZoomLevel;
		d_ptr->m_activePreset = sourceCamera.d_ptr->m_activePreset;
	}

	/*!
	* \property QCamera3D::xRotation
	*
	* \brief The X-rotation angle of the camera around the target point in degrees.
	*/
	float QCamera3D::xRotation() const {
		return d_ptr->m_xRotation;
	}

	void QCamera3D::setXRotation(float rotation)
	{
		if (d_ptr->m_wrapXRotation) {
			rotation = QUtils3D::wrapValue(rotation, d_ptr->m_minXRotation, d_ptr->m_maxXRotation);
		}
		else {
			rotation = qBound(float(d_ptr->m_minXRotation), float(rotation),
				float(d_ptr->m_maxXRotation));
		}

		if (d_ptr->m_xRotation != rotation) {
			d_ptr->setXRotation(rotation);
			if (d_ptr->m_activePreset != CameraPresetNone) {
				d_ptr->m_activePreset = CameraPresetNone;
				setDirty(true);
			}

			emit xRotationChanged(d_ptr->m_xRotation);
		}
	}

	/*!
	* \property QCamera3D::yRotation
	*
	* \brief The Y-rotation angle of the camera around the target point in degrees.
	*/
	float QCamera3D::yRotation() const {
		return d_ptr->m_yRotation;
	}

	void QCamera3D::setYRotation(float rotation)
	{
		if (d_ptr->m_wrapYRotation) {
			rotation = QUtils3D::wrapValue(rotation, d_ptr->m_minYRotation, d_ptr->m_maxYRotation);
		}
		else {
			rotation = qBound(float(d_ptr->m_minYRotation), float(rotation),
				float(d_ptr->m_maxYRotation));
		}

		if (d_ptr->m_yRotation != rotation) {
			d_ptr->setYRotation(rotation);
			if (d_ptr->m_activePreset != CameraPresetNone) {
				d_ptr->m_activePreset = CameraPresetNone;
				setDirty(true);
			}

			emit yRotationChanged(d_ptr->m_yRotation);
		}
	}

	/*!
	* \property QCamera3D::cameraPreset
	*
	* \brief The currently active camera preset.
	*
	* If no CameraPreset value is set, CameraPresetNone is used by default.
	*/
	QCamera3D::CameraPreset QCamera3D::cameraPreset() const
	{
		return d_ptr->m_activePreset;
	}

	void QCamera3D::setCameraPreset(CameraPreset preset)
	{
		switch (preset) {
		case CameraPresetFrontLow: {
			setXRotation(0.0f);
			setYRotation(0.0f);
			break;
		}
		case CameraPresetFront: {
			setXRotation(0.0f);
			setYRotation(22.5f);
			break;
		}
		case CameraPresetFrontHigh: {
			setXRotation(0.0f);
			setYRotation(45.0f);
			break;
		}
		case CameraPresetLeftLow: {
			setXRotation(90.0f);
			setYRotation(0.0f);
			break;
		}
		case CameraPresetLeft: {
			setXRotation(90.0f);
			setYRotation(22.5f);
			break;
		}
		case CameraPresetLeftHigh: {
			setXRotation(90.0f);
			setYRotation(45.0f);
			break;
		}
		case CameraPresetRightLow: {
			setXRotation(-90.0f);
			setYRotation(0.0f);
			break;
		}
		case CameraPresetRight: {
			setXRotation(-90.0f);
			setYRotation(22.5f);
			break;
		}
		case CameraPresetRightHigh: {
			setXRotation(-90.0f);
			setYRotation(45.0f);
			break;
		}
		case CameraPresetBehindLow: {
			setXRotation(180.0f);
			setYRotation(0.0f);
			break;
		}
		case CameraPresetBehind: {
			setXRotation(180.0f);
			setYRotation(22.5f);
			break;
		}
		case CameraPresetBehindHigh: {
			setXRotation(180.0f);
			setYRotation(45.0f);
			break;
		}
		case CameraPresetIsometricLeft: {
			setXRotation(45.0f);
			setYRotation(22.5f);
			break;
		}
		case CameraPresetIsometricLeftHigh: {
			setXRotation(45.0f);
			setYRotation(45.0f);
			break;
		}
		case CameraPresetIsometricRight: {
			setXRotation(-45.0f);
			setYRotation(22.5f);
			break;
		}
		case CameraPresetIsometricRightHigh: {
			setXRotation(-45.0f);
			setYRotation(45.0f);
			break;
		}
		case CameraPresetDirectlyAbove: {
			setXRotation(0.0f);
			setYRotation(90.0f);
			break;
		}
		case CameraPresetDirectlyAboveCW45: {
			setXRotation(-45.0f);
			setYRotation(90.0f);
			break;
		}
		case CameraPresetDirectlyAboveCCW45: {
			setXRotation(45.0f);
			setYRotation(90.0f);
			break;
		}
		case CameraPresetFrontBelow: {
			setXRotation(0.0f);
			setYRotation(-45.0f);
			break;
		}
		case CameraPresetLeftBelow: {
			setXRotation(90.0f);
			setYRotation(-45.0f);
			break;
		}
		case CameraPresetRightBelow: {
			setXRotation(-90.0f);
			setYRotation(-45.0f);
			break;
		}
		case CameraPresetBehindBelow: {
			setXRotation(180.0f);
			setYRotation(-45.0f);
			break;
		}
		case CameraPresetDirectlyBelow: {
			setXRotation(0.0f);
			setYRotation(-90.0f);
			break;
		}
		default:
			preset = CameraPresetNone;
			break;
		}

		// All presets target the center of the graph
		setTarget(zeroVector);

		if (d_ptr->m_activePreset != preset) {
			d_ptr->m_activePreset = preset;
			setDirty(true);
			emit cameraPresetChanged(preset);
		}
	}

	/*!
	* \property QCamera3D::zoomLevel
	*
	* \brief The camera zoom level in percentage.
	*
	* The default value of \c{100.0f} means there is no zoom in or out set in the
	* camera. The value is limited by the minZoomLevel and maxZoomLevel properties.
	*
	* \sa minZoomLevel, maxZoomLevel
	*/
	float QCamera3D::zoomLevel() const
	{
		return d_ptr->m_zoomLevel;
	}

	void QCamera3D::setZoomLevel(float zoomLevel)
	{
		float newZoomLevel = qBound(d_ptr->m_minZoomLevel, zoomLevel, d_ptr->m_maxZoomLevel);

		if (d_ptr->m_zoomLevel != newZoomLevel) {
			d_ptr->m_zoomLevel = newZoomLevel;
			setDirty(true);
			emit zoomLevelChanged(newZoomLevel);
		}
	}

	/*!
	* \property QCamera3D::minZoomLevel
	*
	* \brief The minimum allowed camera zoom level.
	*
	* If the minimum level is set to a new value that is higher than the existing
	* maximum level, the maximum level is adjusted to the new minimum as well.
	* If the current zoomLevel is outside the new bounds, it is adjusted as well.
	* The minZoomLevel cannot be set below \c{1.0f}.
	* Defaults to \c{10.0f}.
	*
	* \sa zoomLevel, maxZoomLevel
	*/
	float QCamera3D::minZoomLevel() const
	{
		return d_ptr->m_minZoomLevel;
	}

	void QCamera3D::setMinZoomLevel(float zoomLevel)
	{
		// Don't allow minimum to be below one, as that can cause zoom to break.
		float newMinLevel = qMax(zoomLevel, 1.0f);
		if (d_ptr->m_minZoomLevel != newMinLevel) {
			d_ptr->m_minZoomLevel = newMinLevel;
			if (d_ptr->m_maxZoomLevel < newMinLevel)
				setMaxZoomLevel(newMinLevel);
			setZoomLevel(d_ptr->m_zoomLevel);
			setDirty(true);
			emit minZoomLevelChanged(newMinLevel);
		}
	}

	/*!
	* \property QCamera3D::maxZoomLevel
	*
	* \brief The maximum allowed camera zoom level.
	*
	* If the maximum level is set to a new value that is lower than the existing
	* minimum level, the minimum level is adjusted to the new maximum as well.
	* If the current zoomLevel is outside the new bounds, it is adjusted as well.
	* Defaults to \c{500.0f}.
	*
	* \sa zoomLevel, minZoomLevel
	*/
	float QCamera3D::maxZoomLevel() const
	{
		return d_ptr->m_maxZoomLevel;
	}

	void QCamera3D::setMaxZoomLevel(float zoomLevel)
	{
		// Don't allow maximum to be below one, as that can cause zoom to break.
		float newMaxLevel = qMax(zoomLevel, 1.0f);
		if (d_ptr->m_maxZoomLevel != newMaxLevel) {
			d_ptr->m_maxZoomLevel = newMaxLevel;
			if (d_ptr->m_minZoomLevel > newMaxLevel)
				setMinZoomLevel(newMaxLevel);
			setZoomLevel(d_ptr->m_zoomLevel);
			setDirty(true);
			emit maxZoomLevelChanged(newMaxLevel);
		}
	}

	/*!
	* \property QCamera3D::wrapXRotation
	*
	* \brief The behavior of the minimum and maximum limits in the X-rotation.
	*
	* If set to \c true, the X-rotation of the camera is wrapped from minimum to
	* maximum and from maximum to minimum. If set to \c false, the X-rotation of
	* the camera is limited to the sector determined by the minimum and maximum
	* values. Set to \c true by default.
	*/
	bool QCamera3D::wrapXRotation() const
	{
		return d_ptr->m_wrapXRotation;
	}

	void QCamera3D::setWrapXRotation(bool isEnabled)
	{
		d_ptr->m_wrapXRotation = isEnabled;
	}

	/*!
	* \property QCamera3D::wrapYRotation
	*
	* \brief The behavior of the minimum and maximum limits in the Y-rotation.
	*
	* If \c true, the Y-rotation of the camera is wrapped from minimum to maximum
	* and from maximum to minimum. If \c false, the Y-rotation of the camera is
	* limited to the sector determined by the minimum and maximum values.
	* Set to \c true by default.
	*/
	bool QCamera3D::wrapYRotation() const
	{
		return d_ptr->m_wrapYRotation;
	}

	void QCamera3D::setWrapYRotation(bool isEnabled)
	{
		d_ptr->m_wrapYRotation = isEnabled;
	}

	/*!
	* Utility function that sets the camera rotations and distance.\a horizontal and \a vertical
	* define the camera rotations to be used.
	* Optional \a zoom parameter can be given to set the zoom percentage of the camera within
	* the bounds defined by minZoomLevel and maxZoomLevel properties.
	*/
	void QCamera3D::setCameraPosition(float horizontal, float vertical, float zoom)
	{
		setZoomLevel(zoom);
		setXRotation(horizontal);
		setYRotation(vertical);
	}

	/*!
	* \property QCamera3D::target
	* \since QtDataVisualization 1.2
	*
	* \brief The camera target as a a vector or vertex in the 3D space.
	*
	* Defaults to \c {QVector3D(0.0, 0.0, 0.0)}.
	*
	* Valid coordinate values are between \c{-1.0...1.0}, where the edge values indicate
	* the edges of the corresponding axis range. Any values outside this range are clamped to the edge.
	*
	* \note For bar graphs, the Y-coordinate is ignored and camera always targets a point on
	* the horizontal background.
	*/
	QVector3D QCamera3D::target() const
	{
		return d_ptr->m_requestedTarget;
	}

	void QCamera3D::setTarget(const QVector3D &target)
	{
		QVector3D newTarget = target;

		if (newTarget.x() < -1.0f)
			newTarget.setX(-1.0f);
		else if (newTarget.x() > 1.0f)
			newTarget.setX(1.0f);

		if (newTarget.y() < -1.0f)
			newTarget.setY(-1.0f);
		else if (newTarget.y() > 1.0f)
			newTarget.setY(1.0f);

		if (newTarget.z() < -1.0f)
			newTarget.setZ(-1.0f);
		else if (newTarget.z() > 1.0f)
			newTarget.setZ(1.0f);

		if (d_ptr->m_requestedTarget != newTarget) {
			if (d_ptr->m_activePreset != CameraPresetNone)
				d_ptr->m_activePreset = CameraPresetNone;
			d_ptr->m_requestedTarget = newTarget;
			setDirty(true);
			emit targetChanged(newTarget);
		}
	}

	QCamera3DPrivate::QCamera3DPrivate(QCamera3D *q) :
		q_ptr(q),
		m_isViewMatrixUpdateActive(true),
		m_xRotation(0.0f),
		m_yRotation(0.0f),
		m_minXRotation(-180.0f),
		m_minYRotation(0.0f),
		m_maxXRotation(180.0f),
		m_maxYRotation(90.0f),
		m_zoomLevel(100.0f),
		m_minZoomLevel(10.0f),
		m_maxZoomLevel(500.0f),
		m_wrapXRotation(true),
		m_wrapYRotation(false),
		m_activePreset(QCamera3D::CameraPresetNone)
	{
	}

	QCamera3DPrivate::~QCamera3DPrivate()
	{
	}

	// Copies changed values from this camera to the other camera. If the other camera had same changes,
	// those changes are discarded.
	void QCamera3DPrivate::sync(QCamera3D &other)
	{
		if (q_ptr->isDirty()) {
			other.copyValuesFrom(*q_ptr);
			q_ptr->setDirty(false);
			other.setDirty(false);
		}
	}

	void QCamera3DPrivate::setXRotation(const float rotation)
	{
		if (m_xRotation != rotation) {
			m_xRotation = rotation;
			q_ptr->setDirty(true);
		}
	}

	void QCamera3DPrivate::setYRotation(const float rotation)
	{
		if (m_yRotation != rotation) {
			m_yRotation = rotation;
			q_ptr->setDirty(true);
		}
	}

	/*!
	* \internal
	* The current minimum X-rotation for the camera.
	* The full circle range is \c{[-180, 180]} and the minimum value is limited to \c -180.
	* Also the value can't be higher than the maximum, and is adjusted if necessary.
	*
	* \sa wrapXRotation, maxXRotation
	*/
	float QCamera3DPrivate::minXRotation() const
	{
		return m_minXRotation;
	}

	void QCamera3DPrivate::setMinXRotation(float minRotation)
	{
		minRotation = qBound(-180.0f, minRotation, 180.0f);
		if (minRotation > m_maxXRotation)
			minRotation = m_maxXRotation;

		if (m_minXRotation != minRotation) {
			m_minXRotation = minRotation;
			emit minXRotationChanged(minRotation);

			if (m_xRotation < m_minXRotation)
				setXRotation(m_xRotation);
			q_ptr->setDirty(true);
		}
	}

	/*!
	* \internal
	* The current minimum Y-rotation for the camera.
	* The full Y angle range is \c{[-90, 90]} and the minimum value is limited to \c -90.
	* Also the value can't be higher than the maximum, and is adjusted if necessary.
	*
	* \sa wrapYRotation, maxYRotation
	*/
	float QCamera3DPrivate::minYRotation() const
	{
		return m_minYRotation;
	}

	void QCamera3DPrivate::setMinYRotation(float minRotation)
	{
		minRotation = qBound(-90.0f, minRotation, 90.0f);
		if (minRotation > m_maxYRotation)
			minRotation = m_maxYRotation;

		if (m_minYRotation != minRotation) {
			m_minYRotation = minRotation;
			emit minYRotationChanged(minRotation);

			if (m_yRotation < m_minYRotation)
				setYRotation(m_yRotation);
			q_ptr->setDirty(true);
		}
	}

	/*!
	* \internal
	* The current maximum X-rotation for the camera.
	* The full circle range is \c{[-180, 180]} and the maximum value is limited to \c 180.
	* Also the value can't be lower than the minimum, and is adjusted if necessary.
	*
	* \sa wrapXRotation, minXRotation
	*/
	float QCamera3DPrivate::maxXRotation() const
	{
		return m_maxXRotation;
	}

	void QCamera3DPrivate::setMaxXRotation(float maxRotation)
	{
		maxRotation = qBound(-180.0f, maxRotation, 180.0f);

		if (maxRotation < m_minXRotation)
			maxRotation = m_minXRotation;

		if (m_maxXRotation != maxRotation) {
			m_maxXRotation = maxRotation;
			emit maxXRotationChanged(maxRotation);

			if (m_xRotation > m_maxXRotation)
				setXRotation(m_xRotation);
			q_ptr->setDirty(true);
		}
	}

	/*!
	* \internal
	* The current maximum Y-rotation for the camera.
	* The full Y angle range is \c{[-90, 90]} and the maximum value is limited to \c 90.
	* Also the value can't be lower than the minimum, and is adjusted if necessary.
	*
	* \sa wrapYRotation, minYRotation
	*/
	float QCamera3DPrivate::maxYRotation() const
	{
		return m_maxYRotation;
	}

	void QCamera3DPrivate::setMaxYRotation(float maxRotation)
	{
		maxRotation = qBound(-90.0f, maxRotation, 90.0f);

		if (maxRotation < m_minYRotation)
			maxRotation = m_minYRotation;

		if (m_maxYRotation != maxRotation) {
			m_maxYRotation = maxRotation;
			emit maxYRotationChanged(maxRotation);

			if (m_yRotation > m_maxYRotation)
				setYRotation(m_yRotation);
			q_ptr->setDirty(true);
		}
	}

	// Recalculates the view matrix based on the currently set base orientation, rotation and zoom level values.
	//  zoomAdjustment is adjustment to ensure that the 3D visualization stays inside the view area in the 100% zoom.
	void QCamera3DPrivate::updateViewMatrix(float zoomAdjustment)
	{
		if (!m_isViewMatrixUpdateActive)
			return;

		GLfloat zoom = m_zoomLevel * zoomAdjustment;
		QMatrix4x4 viewMatrix;

		// Apply to view matrix
		viewMatrix.lookAt(q_ptr->position(), m_actualTarget, m_up);
		// Compensate for translation (if d_ptr->m_target is off origin)
		viewMatrix.translate(m_actualTarget.x(), m_actualTarget.y(), m_actualTarget.z());
		// Apply rotations
		// Handle x and z rotation when y -angle is other than 0
		viewMatrix.rotate(m_xRotation, 0, qCos(qDegreesToRadians(m_yRotation)),
			qSin(qDegreesToRadians(m_yRotation)));
		// y rotation is always "clean"
		viewMatrix.rotate(m_yRotation, 1.0f, 0.0f, 0.0f);
		// handle zoom by scaling
		viewMatrix.scale(zoom / 100.0f);
		// Compensate for translation (if d_ptr->m_target is off origin)
		viewMatrix.translate(-m_actualTarget.x(), -m_actualTarget.y(), -m_actualTarget.z());

		setViewMatrix(viewMatrix);
	}

	/*!
	* \internal
	* The view matrix used in the 3D calculations. When the default orbiting
	* camera behavior is sufficient, there is no need to touch this property. If the default
	* behavior is insufficient, the view matrix can be set directly.
	* \note When setting the view matrix directly remember to set viewMatrixAutoUpdateEnabled to
	* \c false.
	*/
	QMatrix4x4 QCamera3DPrivate::viewMatrix() const
	{
		return m_viewMatrix;
	}

	void QCamera3DPrivate::setViewMatrix(const QMatrix4x4 &viewMatrix)
	{
		if (m_viewMatrix != viewMatrix) {
			m_viewMatrix = viewMatrix;
			q_ptr->setDirty(true);
			emit viewMatrixChanged(m_viewMatrix);
		}
	}

	/*!
	* \internal
	* This property determines if view matrix is automatically updated each render cycle using the
	* current base orientation and rotations. If set to \c false, no automatic recalculation is done
	* and the view matrix can be set using the viewMatrix property.
	*/
	bool QCamera3DPrivate::isViewMatrixAutoUpdateEnabled() const
	{
		return m_isViewMatrixUpdateActive;
	}

	void QCamera3DPrivate::setViewMatrixAutoUpdateEnabled(bool isEnabled)
	{
		m_isViewMatrixUpdateActive = isEnabled;
		emit viewMatrixAutoUpdateChanged(isEnabled);
	}

	/*!
	* \internal
	* Sets the base values for the camera that are used when calculating the camera position using the
	* rotation values. The base position of the camera is defined by \a basePosition, expectation is
	* that the x and y values are 0. Look at target point is defined by \a target and the camera
	* rotates around it. Up direction for the camera is defined by \a baseUp, normally this is a
	* vector with only y value set to 1.
	*/
	void QCamera3DPrivate::setBaseOrientation(const QVector3D &basePosition,
		const QVector3D &target,
		const QVector3D &baseUp)
	{
		if (q_ptr->position() != basePosition || m_actualTarget != target || m_up != baseUp) {
			q_ptr->setPosition(basePosition);
			m_actualTarget = target;
			m_up = baseUp;
			q_ptr->setDirty(true);
		}
	}

	/*!
	* \internal
	* Calculates and returns a position relative to the camera using the given parameters
	* and the current camera viewMatrix property.
	* The relative 3D offset to the current camera position is defined in \a relativePosition.
	* An optional fixed rotation of the calculated point around the data visualization area can be
	* given in \a fixedRotation. The rotation is given in degrees.
	* An optional \a distanceModifier modifies the distance of the calculated point from the data
	* visualization.
	* \return calculated position relative to this camera's position.
	*/
	QVector3D QCamera3DPrivate::calculatePositionRelativeToCamera(const QVector3D &relativePosition,
		float fixedRotation,
		float distanceModifier) const
	{
		// Move the position with camera
		const float radiusFactor = cameraDistance * (1.5f + distanceModifier);
		float xAngle;
		float yAngle;

		if (!fixedRotation) {
			xAngle = qDegreesToRadians(m_xRotation);
			float yRotation = m_yRotation;
			// Light must not be paraller to eye vector, so fudge the y rotation a bit.
			// Note: This needs redoing if we ever allow arbitrary light positioning.
			const float yMargin = 0.1f; // Smaller margins cause weird shadow artifacts on tops of bars
			const float absYRotation = qAbs(yRotation);
			if (absYRotation < 90.0f + yMargin && absYRotation > 90.0f - yMargin) {
				if (yRotation < 0.0f)
					yRotation = -90.0f + yMargin;
				else
					yRotation = 90.0f - yMargin;
			}
			yAngle = qDegreesToRadians(yRotation);
		}
		else {
			xAngle = qDegreesToRadians(fixedRotation);
			yAngle = 0;
		}
		// Set radius to match the highest height of the position
		const float radius = (radiusFactor + relativePosition.y());
		const float zPos = radius * qCos(xAngle) * qCos(yAngle);
		const float xPos = radius * qSin(xAngle) * qCos(yAngle);
		const float yPos = radius * qSin(yAngle);

		// Keep in the set position in relation to camera
		return QVector3D(-xPos + relativePosition.x(),
			yPos + relativePosition.y(),
			zPos + relativePosition.z());
	}
}