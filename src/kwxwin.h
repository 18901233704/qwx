/**
 * Copyright (C) 2009 Ben Cooksley <bcooksley@kde.org>
 * Copyright (C) 2016 Leslie Zhai <xiang.zhai@i-soft.com.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef KWXWIN_H
#define KWXWIN_H

#include <KXmlGuiWindow>

#include <QQuickWidget>

class KwxWin : public KXmlGuiWindow
{
	Q_OBJECT

public:
    explicit KwxWin(QWidget *parent = Q_NULLPTR);
    ~KwxWin();

private:
    QQuickWidget *m_widget = Q_NULLPTR;
};

#endif
