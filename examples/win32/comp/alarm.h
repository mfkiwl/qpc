/*****************************************************************************
* Product: Orthogonal Component state pattern example
* Last updated for version 5.4.0
* Last updated on  2015-03-25
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
#ifndef alarm_h
#define alarm_h

typedef struct {   /* the FSM version of the Alarm component */
    QFsm super;    /* inherit FHsm */
    uint32_t alarm_time;
} Alarm;

void Alarm_ctor(Alarm * const me);
#define Alarm_init(me_)           QMSM_INIT    ((QMsm *)(me_), (QEvt *)0)
#define Alarm_dispatch(me_, e_)   QMSM_DISPATCH((QMsm *)(me_), e_)

#endif /* alarm_h */
