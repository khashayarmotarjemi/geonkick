/**
 * File name: gkick_envelope.cpp
 * Project: GeonKick
 *
 * Copyright (C) 2015 Iurie Nistor
 * Email: nistor@iurie.org
 *
 * This file is part of GeonKick.
 *
 * waveToMp3 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "gkick_oscillator.h"
#include "oscillator_envelope.h"
#include <QDebug>

GKickEnvelope::GKickEnvelope(QWidget *parent)
        : parentWidget(parent),
          name("GKickEnvelope")
          envelopeType(GKickEnvelope::ENV_TYPE_AMPLITUDE);
          envelopeCategory(GKickEnvelope::ENV_CATEGORY_GENERAL);
{

}

GKickEnvelope::~GKickEnvelope()
{
}

void GKickEnvelope::draw(QPainter &painter)
{
	drawPoints(painter);
	drawLines(painter);
}

void GKickEnvelope::setXRatio(double k)
{
        xRatio = k;
}

void GKickEnvelope::setYRatio(double k)
{
        yRatio = k;
}

void GKickEnvelope::drawPoints(QPainter &painter)
{
	for (int i = 0; i < envelopePoints.size(); i++) {
		envelopePoints[i].draw(painter);
	}
}

void OscillatorWidget::drawAxes(QPainter &painter)
{
        QPen pen(Qt::black);
        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawLine(xPadding, yPadding, xPadding, height() - yPadding);
        painter.drawLine(xPadding, height() - yPadding,
                         width() - xPadding, height() - yPadding);
}

void GKickEnvelope::drawLines(QPainter &painter)
{
	QPolygonF points;
	for (int i = 0; i < envelopePoints.size(); i++) {
	        QPointF p = scaleUp(envelopePoints[i]);
		points << QPointF(originPoint.x() + p.x(),
				  originPoint.y() - p.y());
	}

	QPen pen = painter.pen();
	pen.setWidth(1);
	painter.setPen(pen);
	painter.drawPolyline(points);
}

void GKickEnvelope::setOrigin(QPointF &point)
{
	originPoint = point;
}

QPointF GKickEnvelope::getOriginPoint(void)
{
        return originPoint;
}

bool GKickEnvelope::hasSelected(void)
{
	return selectedPoint != NULL;
}

void GKickEnvelope::selectPoint(QPointF point)
{
	for(int i = 0; i < envelopePoints.size(); i++) {
		if (envelopePoints[i].hasPoint(point)) {
			selectedPoint = &envelopePoints[i];
			selectedPoint->selectPoint();
			break;
		}
	}
}

void GKickEnvelope::unselectPoint(void)
{
	if (selectedPoint) {
		selectedPoint->unselectPoint();
		selectedPoint = NULL;
	}
}

double GKickEnvelope::getLeftPointLimit(void)
{
	if (selectedPoint == NULL) {
		return 0.0;
	}

	double x = 0.0;
	int index = envelopePoints.indexOf(*selectedPoint);
	if (index == -1) {
		x = 0.0;
	} else if (envelopePoints[index] == envelopePoints.first()) {
		x = 0.0;
	} else {
		x = envelopePoints[index - 1].x();
	}

	return x;
}

double GKickEnvelope::getRightPointLimit(void)
{
	if (selectedPoint == NULL) {
		return 0.0;
	}

	double x = 0.0;
	int index = envelopePoints.indexOf(*selectedPoint);
	if (index == -1) {
		x = 0.0;
	} else if (envelopePoints[index] == envelopePoints.last()) {
		x = envelopeW;
	} else {
		x = envelopePoints[index + 1].x();
	}

	return x;
}

void GKickEnvelope::moveSelectedPoint(double x, double y)
{
	if (selectedPoint == NULL) {
		return;
	}

	x /= xRatio;
	y /= yRatio;

	if (x < getLeftPointLimit()) {
                selectedPoint->setX(getLeftPointLimit());
	} else if (x > getRightPointLimit()) {
                selectedPoint->setX(getRightPointLimit());
	} else {
                selectedPoint->setX(x);
	}

	if (y < 0.0) {
                selectedPoint->setY(0.0);
	} else if (y > envelopeH) {
                selectedPoint->setY(envelopeH);
	} else {
                selectedPoint->setY(y);
	}

	int index = envelopePoints.indexOf(*selectedPoint);
	if (index > -1 && index < envelopePoints.size()) {
                pointUpdatedEvent(index, QPointF(selectedPoint->x(), selectedPoint->y()));
	}
}

void GKickEnvelope::addEnvelopePoints(QPolygonF points)
{
        for (int i = 0; i < points.size(); i++) {
                envelopePoints.push_back(GKickEnvelopePoint(this, points[i]));
        }
}

void GKickEnvelope::removePoints(void)
{
        envelopePoints.clear();
}

QPointF GKickEnvelope::scaleUp(QPointF point)
{
        point.setX(point.x() * xRatio);
        point.setY(point.y() * yRatio);
        return point;
}

QPointF GKickEnvelope::scaleDown(QPointF point)
{
        point.setX(point.x() / xRatio);
        point.setY(point.y() / yRatio);
        return point;
}

void GKickEnvelope::addPoint(QPointF point)
{
        point = scaleDown(point);

        if (point.y() < 0.0) {
                point.setY(0.0);
        }  else if (point.y() > envelopeH) {
                point.setY(envelopeH);
        }

  	if (point.x() > envelopeW) {
	        point.setX(envelopeW);
		envelopePoints.append(GKickEnvelopePoint(this, point));
	} else if (point.x() < 0.0) {
                point.setX(0.0);
                envelopePoints.push_front(GKickEnvelopePoint(this, point));
	} else if (point.x() < envelopePoints[0].x()) {
                envelopePoints.push_front(GKickEnvelopePoint(this, point));
	} else if (point.x() > envelopePoints.last().x()) {
                envelopePoints.push_back(GKickEnvelopePoint(this, point));
	} else {
		GKickEnvelopePoint p;
		for(int i = 0; i < envelopePoints.size(); i++) {
			if (point.x() < envelopePoints[i].x()) {
                                envelopePoints.insert(i, GKickEnvelopePoint(this, point));
				break;
			}
		}
	}

	pointAddedEvent(point.x(), point.y());
}

double GKickEnvelope::getEnvelopeLenth(void)
{
        return envelopeW;
}

double GKickEnvelope::getEnvelopeHeight(void)
{
        return envelopeH;
}

void GKickEnvelope::removePoint(QPointF point)
{
	for(int i = 0; i < envelopePoints.size(); i++) {
		if (envelopePoints[i].hasPoint(point)) {
			if (envelopePoints[i] != envelopePoints.first()
			    && envelopePoints[i] != envelopePoints.last()) {
				envelopePoints.remove(i);
				pointRemovedEvent(i);
			}
			break;
		}
	}
}

double GKickEnvelope::getKickLength(void)
{
        return parentWidget->getKickLength();
}

void GKickEnvelope::setEnvelopeValue(double v)
{
	envelopeValue = v;
}

double GKickEnvelope::getEnvelopeValue(void)
{
        return envelopeValue;
}

void GKickEnvelope::setType(GKickEnvelope::EnvelopeType type)
{
	envelopeType = type;
}

GKickEnvelope::EnvelopeType GKickEnvelope::type(void)
{
	return envelopeType;
}

void GKickEnvelope::setCategory(GKickEnvelope::EnvelopeCategory cat)
{
	envelopeCategory = cat;
}

GKickEnvelope::EnvelopeCategory GKickEnvelope::category(void)
{
	return envelopeCategory;
}

QString GKickEnvelope::name(void)
{
        return envelopeName;
}
