/**
 * File name: gkick_envelope.h
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of GeonKick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
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
#include "globals.h"

GKickEnvelope::GKickEnvelope()
:        selectedPointIndex(0),
         pointSelected(false),
         envelopeType(GKickEnvelope::ENV_TYPE_AMPLITUDE),
         envelopeCategory(GKickEnvelope::ENV_CATEGORY_GENERAL),
         envelopeName("GKick Envelope"),
         envelopeOrigin(0, 0),
         envelopeW(0),
         envelopeH(0)
{

}

GKickEnvelope::~GKickEnvelope()
{
}

int GKickEnvelope::W(void) const
{
        return envelopeW;
}

int GKickEnvelope::H(void) const
{
        return envelopeH;
}

QPointF GKickEnvelope::origin(void) const
{
        return envelopeOrigin;
}

void GKickEnvelope::draw(QPainter &painter, const QRectF &rect)
{
        envelopeW = rect.width();
        envelopeH = rect.height();
        envelopeOrigin = rect.bottomLeft();
        drawScale(painter);
        drawPoints(painter);
        drawLines(painter);
}

void GKickEnvelope::drawScale(QPainter &painter)
{
        auto val = envelopeLengh() / 10;
        auto dx = W() / 10;
        auto x = (envelopeOrigin.x()) + dx;
        QPen pen = painter.pen();
        painter.setPen(QPen(QColor(110, 110, 110)));
        QFont font = painter.font();
        font.setPixelSize(10);
        painter.setFont(font);
        for (auto i = 1; i < 10; i++) {
                QRect rect(x - 10, envelopeOrigin.y() - 12, 20, font.pixelSize());
                painter.drawText(rect, Qt::AlignCenter, QString::number(i * val));
                x += dx;
        }

        font.setPixelSize(12);
        painter.setFont(font);
        painter.setPen(QPen(QColor(180, 180, 180, 200)));
        painter.drawText(envelopeOrigin.x() + W() / 2 - 35, envelopeOrigin.y() + 20,
                         tr("Length, ") + QString::number(envelopeLengh(), 'f', 0) + " ms");

        /*        QString text;
        if (type() != EnvelopeType::ENV_TYPE_AMPLITUDE) {
                text = tr("Amplitude, dB");
        } else {
                text = tr("Frequency, Hz");
        }
        p.translate(envelopeOrigin.x() - 30, envelopeOrigin.y() - H() / 2 + 35);
        p.rotate(-90);
        p.drawText(0, 0, text);
        p.setPen(pen);*/
}

void GKickEnvelope::drawPoints(QPainter &painter)
{
	for (const auto &point : envelopePoints) {
		point->draw(painter);
        }
}

void GKickEnvelope::drawLines(QPainter &painter)
{
        QPolygonF points;
	for (const auto& point : envelopePoints) {
	        points << QPointF(envelopeOrigin.x() + point->x() * W(),
                                  envelopeOrigin.y() - (point->y() * H()));
	}

	auto pen = painter.pen();
	pen.setWidth(2);
        pen.setColor(QColor(200, 200, 200, 200));
	painter.setPen(pen);
        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing, true);
	painter.drawPolyline(points);
}

bool GKickEnvelope::hasSelected(void) const
{
	return pointSelected;
}

void GKickEnvelope::selectPoint(const QPointF &point)
{
        std::vector<std::shared_ptr<GKickEnvelopePoint>>::size_type index = 0;
	for (const auto& p : envelopePoints) {
		if (p->hasPoint(point)) {
			p->selectPoint();
                        selectedPointIndex = index;
                        pointSelected = true;
			break;
		}
                index++;
	}
}

void GKickEnvelope::unselectPoint(void)
{
        if (pointSelected) {
		envelopePoints[selectedPointIndex]->unselectPoint();
                pointSelected = false;
        }
}

double GKickEnvelope::getLeftPointLimit(void) const
{
        double x;
	if (!pointSelected || envelopePoints.empty() || selectedPointIndex < 1) {
		x = 0.0;
	} else {
		x = envelopePoints[selectedPointIndex - 1]->x();
	}

	return x;
}

double GKickEnvelope::getRightPointLimit(void) const
{
	double x;
	if (!pointSelected || envelopePoints.empty()) {
		x = 0.0;
	} else if (selectedPointIndex >= envelopePoints.size() - 1) {
		x = 1.0;
	} else {
		x = envelopePoints[selectedPointIndex + 1]->x();
        }

	return x;
}

void GKickEnvelope::moveSelectedPoint(double x, double y)
{
        if (!pointSelected || envelopePoints.empty()) {
		return;
	}

	x /= W();
	y /= H();

        auto selectedPoint = envelopePoints[selectedPointIndex];
	if (x < getLeftPointLimit()) {
                selectedPoint->setX(getLeftPointLimit());
	} else if (x > getRightPointLimit()) {
                selectedPoint->setX(getRightPointLimit());
	} else {
                selectedPoint->setX(x);
	}

	if (y < 0.0) {
                selectedPoint->setY(0.0);
	} else if (y > 1) {
                selectedPoint->setY(1.0);
	} else {
                selectedPoint->setY(y);
	}

        pointUpdatedEvent(selectedPointIndex,
                          selectedPoint->x(),
                          selectedPoint->y());
}

void GKickEnvelope::addPoints(const QPolygonF &points)
{
        for (const auto &point : points) {
                envelopePoints.push_back(std::make_shared<GKickEnvelopePoint>(this, point));
        }
}

void GKickEnvelope::addPoint(const QPointF &point)
{
        QPointF scaledPoint = QPointF(point.x() / W(), point.y() / H());

        if (scaledPoint.y() < 0.0) {
                scaledPoint.setY(0.0);
        }  else if (scaledPoint.y() > 1.0) {
                scaledPoint.setY(1.0);
        }

  	if (scaledPoint.x() > 1.0) {
	        scaledPoint.setX(1.0);
		envelopePoints.push_back(std::make_shared<GKickEnvelopePoint>(this, scaledPoint));
	} else if (scaledPoint.x() < 0.0) {
                scaledPoint.setX(0.0);
                envelopePoints.insert(envelopePoints.begin(),
                                      std::make_shared<GKickEnvelopePoint>(this, scaledPoint));
        } else if (envelopePoints.empty()) {
                envelopePoints.push_back(std::make_shared<GKickEnvelopePoint>(this, scaledPoint));
	} else if (scaledPoint.x() < envelopePoints[0]->x()) {
                envelopePoints.insert(envelopePoints.begin(),
                                      std::make_shared<GKickEnvelopePoint>(this, scaledPoint));
	} else if (scaledPoint.x() > envelopePoints.back()->x()) {
                envelopePoints.push_back(std::make_shared<GKickEnvelopePoint>(this, scaledPoint));
	} else {
		for (auto it = envelopePoints.begin() ; it != envelopePoints.end(); ++it) {
			if (scaledPoint.x() < (*it)->x()) {
                                envelopePoints.insert(it, std::make_shared<GKickEnvelopePoint>(this, scaledPoint));
                                break;
			}
		}
	}

	pointAddedEvent(scaledPoint.x(), scaledPoint.y());
}

void GKickEnvelope::removePoint(QPointF point)
{
        std::vector<std::shared_ptr<GKickEnvelopePoint>>::size_type index = 0;
        for (const auto p: envelopePoints) {
		if (p->hasPoint(point)) {
			if (p != envelopePoints.front() && p != envelopePoints.back()) {
				envelopePoints.erase(envelopePoints.begin() + index);
				pointRemovedEvent(index);
			}
			break;
		}
                index++;
        }
}

void GKickEnvelope::setType(GKickEnvelope::EnvelopeType type)
{
	envelopeType = type;
}

GKickEnvelope::EnvelopeType GKickEnvelope::type(void) const
{
	return envelopeType;
}

void GKickEnvelope::setCategory(GKickEnvelope::EnvelopeCategory cat)
{
	envelopeCategory = cat;
}

GKickEnvelope::EnvelopeCategory GKickEnvelope::category(void) const
{
	return envelopeCategory;
}

QString GKickEnvelope::name(void) const
{
        return envelopeName;
}
