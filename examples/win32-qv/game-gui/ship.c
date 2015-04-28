/*****************************************************************************
* Model: game.qm
* File:  ./ship.c
*
* This code has been generated by QM tool (see state-machine.com/qm).
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*****************************************************************************/
/*${.::ship.c} .............................................................*/
#include "qpc.h"
#include "bsp.h"
#include "game.h"

/* Q_DEFINE_THIS_FILE */

#define SHIP_WIDTH  5
#define SHIP_HEIGHT 3

/* encapsulated delcaration of the Ship active object ----------------------*/
/*${AOs::Ship} .............................................................*/
typedef struct {
/* protected: */
    QMActive super;

/* private: */
    uint8_t x;
    uint8_t y;
    uint8_t exp_ctr;
    uint16_t score;
} Ship;

/* protected: */
static QState Ship_initial(Ship * const me, QEvt const * const e);
static QState Ship_active  (Ship * const me, QEvt const * const e);
static QState Ship_active_i(Ship * const me);
static QMState const Ship_active_s = {
    (QMState const *)0, /* superstate (top) */
    Q_STATE_CAST(&Ship_active),
    Q_ACTION_CAST(0), /* no entry action */
    Q_ACTION_CAST(0), /* no exit action */
    Q_ACTION_CAST(&Ship_active_i)
};
static QState Ship_parked  (Ship * const me, QEvt const * const e);
static QMState const Ship_parked_s = {
    &Ship_active_s, /* superstate */
    Q_STATE_CAST(&Ship_parked),
    Q_ACTION_CAST(0), /* no entry action */
    Q_ACTION_CAST(0), /* no exit action */
    Q_ACTION_CAST(0)  /* no intitial tran. */
};
static QState Ship_flying  (Ship * const me, QEvt const * const e);
static QState Ship_flying_e(Ship * const me);
static QMState const Ship_flying_s = {
    &Ship_active_s, /* superstate */
    Q_STATE_CAST(&Ship_flying),
    Q_ACTION_CAST(&Ship_flying_e),
    Q_ACTION_CAST(0), /* no exit action */
    Q_ACTION_CAST(0)  /* no intitial tran. */
};
static QState Ship_exploding  (Ship * const me, QEvt const * const e);
static QState Ship_exploding_e(Ship * const me);
static QMState const Ship_exploding_s = {
    &Ship_active_s, /* superstate */
    Q_STATE_CAST(&Ship_exploding),
    Q_ACTION_CAST(&Ship_exploding_e),
    Q_ACTION_CAST(0), /* no exit action */
    Q_ACTION_CAST(0)  /* no intitial tran. */
};


/* local objects -----------------------------------------------------------*/
static Ship l_ship; /* the sole instance of the Ship active object */

/* Public-scope objects ----------------------------------------------------*/
QActive * const AO_Ship = (QActive *)&l_ship; /* opaque pointer */

/* Active object definition ------------------------------------------------*/
/*${AOs::Ship_ctor} ........................................................*/
void Ship_ctor(void) {
    Ship *me = &l_ship;
    QMActive_ctor(&me->super, Q_STATE_CAST(&Ship_initial));
    me->x = GAME_SHIP_X;
    me->y = GAME_SHIP_Y;
}
/*${AOs::Ship} .............................................................*/
/*${AOs::Ship::SM} .........................................................*/
static QState Ship_initial(Ship * const me, QEvt const * const e) {
    static struct {
        QMState const *target;
        QActionHandler act[2];
    } const tatbl_ = { /* transition-action table */
        &Ship_active_s, /* target state */
        {
            Q_ACTION_CAST(&Ship_active_i), /* init.tran. */
            Q_ACTION_CAST(0) /* zero terminator */
        }
    };
    /* ${AOs::Ship::SM::initial} */
    (void)e; /* avoid the compiler warning 'usused parameter' */
    QActive_subscribe((QActive *)me, TIME_TICK_SIG);
    QActive_subscribe((QActive *)me, PLAYER_TRIGGER_SIG);
    /* object dictionaries... */
    QS_OBJ_DICTIONARY(&l_ship);
    /* function dictionaries for Ship HSM... */
    QS_FUN_DICTIONARY(&Ship_initial);
    QS_FUN_DICTIONARY(&Ship_active);
    QS_FUN_DICTIONARY(&Ship_parked);
    QS_FUN_DICTIONARY(&Ship_flying);
    QS_FUN_DICTIONARY(&Ship_exploding);
    /* local signals... */
    QS_SIG_DICTIONARY(PLAYER_SHIP_MOVE_SIG, &l_ship);
    QS_SIG_DICTIONARY(TAKE_OFF_SIG,         &l_ship);
    QS_SIG_DICTIONARY(HIT_WALL_SIG,         &l_ship);
    QS_SIG_DICTIONARY(HIT_MINE_SIG,         &l_ship);
    QS_SIG_DICTIONARY(DESTROYED_MINE_SIG,   &l_ship);
    return QM_TRAN_INIT(&tatbl_);
}
/*${AOs::Ship::SM::active} .................................................*/
/* ${AOs::Ship::SM::active::initial} */
static QState Ship_active_i(Ship * const me) {
    static QMTranActTable const tatbl_ = { /* transition-action table */
        &Ship_parked_s,
        {
            Q_ACTION_CAST(0) /* zero terminator */
        }
    };
    /* ${AOs::Ship::SM::active::initial} */
    return QM_TRAN_INIT(&tatbl_);
}
/* ${AOs::Ship::SM::active} */
static QState Ship_active(Ship * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${AOs::Ship::SM::active::PLAYER_SHIP_MOVE} */
        case PLAYER_SHIP_MOVE_SIG: {
            me->x = Q_EVT_CAST(ObjectPosEvt)->x;
            me->y = Q_EVT_CAST(ObjectPosEvt)->y;
            status_ = QM_HANDLED();
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
/*${AOs::Ship::SM::active::parked} .........................................*/
/* ${AOs::Ship::SM::active::parked} */
static QState Ship_parked(Ship * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${AOs::Ship::SM::active::parked::TAKE_OFF} */
        case TAKE_OFF_SIG: {
            static struct {
                QMState const *target;
                QActionHandler act[2];
            } const tatbl_ = { /* transition-action table */
                &Ship_flying_s, /* target state */
                {
                    Q_ACTION_CAST(&Ship_flying_e), /* entry */
                    Q_ACTION_CAST(0) /* zero terminator */
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
/*${AOs::Ship::SM::active::flying} .........................................*/
/* ${AOs::Ship::SM::active::flying} */
static QState Ship_flying_e(Ship * const me) {
    ScoreEvt *sev;
    me->score = 0U; /* reset the score */
    sev = Q_NEW(ScoreEvt, SCORE_SIG);
    sev->score = me->score;
    QACTIVE_POST(AO_Tunnel, (QEvt *)sev, me);
    return QM_ENTRY(&Ship_flying_s);
}
/* ${AOs::Ship::SM::active::flying} */
static QState Ship_flying(Ship * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${AOs::Ship::SM::active::flying::TIME_TICK} */
        case TIME_TICK_SIG: {
            /* tell the Tunnel to draw the Ship and test for hits */
            ObjectImageEvt *oie = Q_NEW(ObjectImageEvt, SHIP_IMG_SIG);
            oie->x   = me->x;
            oie->y   = me->y;
            oie->bmp = SHIP_BMP;
            QACTIVE_POST(AO_Tunnel, (QEvt *)oie, me);

            ++me->score; /* increment the score for surviving another tick */

            if ((me->score % 10U) == 0U) { /* is the score "round"? */
                ScoreEvt *sev = Q_NEW(ScoreEvt, SCORE_SIG);
                sev->score = me->score;
                QACTIVE_POST(AO_Tunnel, (QEvt *)sev, me);
            }
            status_ = QM_HANDLED();
            break;
        }
        /* ${AOs::Ship::SM::active::flying::PLAYER_TRIGGER} */
        case PLAYER_TRIGGER_SIG: {
            ObjectPosEvt *ope = Q_NEW(ObjectPosEvt, MISSILE_FIRE_SIG);
            ope->x = me->x;
            ope->y = me->y + SHIP_HEIGHT - 1U;
            QACTIVE_POST(AO_Missile, (QEvt *)ope, me);
            status_ = QM_HANDLED();
            break;
        }
        /* ${AOs::Ship::SM::active::flying::DESTROYED_MINE} */
        case DESTROYED_MINE_SIG: {
            me->score += Q_EVT_CAST(ScoreEvt)->score;
            /* the score will be sent to the Tunnel by the next TIME_TICK */
            status_ = QM_HANDLED();
            break;
        }
        /* ${AOs::Ship::SM::active::flying::HIT_WALL} */
        case HIT_WALL_SIG: {
            static struct {
                QMState const *target;
                QActionHandler act[2];
            } const tatbl_ = { /* transition-action table */
                &Ship_exploding_s, /* target state */
                {
                    Q_ACTION_CAST(&Ship_exploding_e), /* entry */
                    Q_ACTION_CAST(0) /* zero terminator */
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        /* ${AOs::Ship::SM::active::flying::HIT_MINE} */
        case HIT_MINE_SIG: {
            static struct {
                QMState const *target;
                QActionHandler act[2];
            } const tatbl_ = { /* transition-action table */
                &Ship_exploding_s, /* target state */
                {
                    Q_ACTION_CAST(&Ship_exploding_e), /* entry */
                    Q_ACTION_CAST(0) /* zero terminator */
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
/*${AOs::Ship::SM::active::exploding} ......................................*/
/* ${AOs::Ship::SM::active::exploding} */
static QState Ship_exploding_e(Ship * const me) {
    me->exp_ctr = 0U;
    return QM_ENTRY(&Ship_exploding_s);
}
/* ${AOs::Ship::SM::active::exploding} */
static QState Ship_exploding(Ship * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${AOs::Ship::SM::active::exploding::TIME_TICK} */
        case TIME_TICK_SIG: {
            /* ${AOs::Ship::SM::active::exploding::TIME_TICK::[me->exp_ctr<15U~} */
            if (me->exp_ctr < 15U) {
                ObjectImageEvt *oie;
                ++me->exp_ctr;
                /* tell the Tunnel to draw the current stage of Explosion */
                oie = Q_NEW(ObjectImageEvt, EXPLOSION_SIG);
                oie->bmp = EXPLOSION0_BMP + (me->exp_ctr >> 2);
                oie->x   = me->x; /* x of explosion */
                oie->y   = (int8_t)((int)me->y - 4U + SHIP_HEIGHT);
                QACTIVE_POST(AO_Tunnel, (QEvt *)oie, me);
                status_ = QM_HANDLED();
            }
            /* ${AOs::Ship::SM::active::exploding::TIME_TICK::[else]} */
            else {
                static QMTranActTable const tatbl_ = { /* transition-action table */
                    &Ship_parked_s,
                    {
                        Q_ACTION_CAST(0) /* zero terminator */
                    }
                };
                ScoreEvt *gameOver = Q_NEW(ScoreEvt, GAME_OVER_SIG);
                gameOver->score = me->score;
                QACTIVE_POST(AO_Tunnel, (QEvt *)gameOver, me);
                status_ = QM_TRAN(&tatbl_);
            }
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

