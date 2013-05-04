/*
 *  QWProgs-MegaTF_C
 *  Copyright (C) 2006  XavioR
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 *  $Id: mtf_scout.c,v 1.00 2006/09/15 XavioR Exp $
 */
#include "g_local.h"
void	TeamFortress_DetpackDisarm( );
// MegaTF scout functions (excluding drop commands)

// Detpack Disarm (drop3)
void TeamFortress_DetpackDisarmTrigger ( )
{
	gedict_t *disarm, *owner;
	vec3_t  source;
	vec3_t  dest;

	if ( tg_data.detpack_drop )
		CheckBelowBuilding( self );

	if ( tg_data.disable_disarm )
		return;

	if ( strneq( other->s.v.classname, "player" ) )
		return;
	if ( other->playerclass != PC_SCOUT )
		return;
	if ( other->s.v.deadflag )
		return;
	if ( self->weaponmode == 1 )
		return;
	owner = PROG_TO_EDICT( self->s.v.owner );

	if ( (owner->team_no && TeamFortress_isTeamsAllied (other->team_no , owner->team_no)) && !tg_data.tg_enabled )
		return;

	makevectors( other->s.v.v_angle );
	VectorCopy( other->s.v.origin, source );
	source[2] += 16;

	VectorScale( g_globalvars.v_forward, 64, dest );
	VectorAdd( source, dest, dest );
	traceline( PASSVEC3( source ), PASSVEC3( dest ), 0, other );
	if ( g_globalvars.trace_fraction == 1 || g_globalvars.trace_ent != EDICT_TO_PROG( self ) )
		return;
	other->tfstate = other->tfstate | TFSTATE_CANT_MOVE;
	G_sprint( other, 2, "Disarming detpack...\n" );
	TeamFortress_SetSpeed( other );
	
	// Spawn disarming entity
	disarm = spawn(  );
	disarm->s.v.owner = EDICT_TO_PROG( other );// the scout
	disarm->s.v.enemy = EDICT_TO_PROG( self ); // the detpack
	disarm->s.v.classname = "timer";
	disarm->s.v.nextthink = g_globalvars.time + WEAP_DETPACK_DISARMTIME;
	disarm->s.v.think = ( func_t ) TeamFortress_DetpackDisarm;
	self->weaponmode = 1;                          // indicates disarming
	self->s.v.enemy = EDICT_TO_PROG( other );      // points to scout
	self->observer_list = disarm;
}

void DetpackDisarm ( )
{
	gedict_t *te;

	te = findradius( world, self->s.v.origin, 38 );
	while ( te )
	{
		if ( streq( te->s.v.classname, "detpack" ) ) {
			if ( te->radsuit_finished < g_globalvars.time ) {
				other = self;
				self = te;
				TeamFortress_DetpackDisarmTrigger ();
				self = other;				
			}
		}
		te = findradius( te, self->s.v.origin, 38 );
	}
}

// JetJump
void SuperLeap ( )
{
	if ( !tf_data.airscout ) {
		G_sprint( self, 1, "JetPack disabled by Admin!\n" );
		return;
	}
	if ( self->s.v.ammo_cells < 24 ) {
		G_sprint( self, 1, "Insufficient power... get more cells\n" );
		return;
	}
	if ( self->tfstate & TFSTATE_CANT_MOVE ) {
		return;
	}
	sound( self, CHAN_ITEM, "weapons/jetjump.wav", 1, ATTN_NORM );
	W_FireFlame( );
	muzzleflash( );
///	self.velocity = v_forward * 900 + '0 0 250' + v_up * 100;
	makevectors( self->s.v.v_angle );
	self->s.v.velocity[0] =
		( g_globalvars.v_forward[0] * 900 + g_globalvars.v_up[0] * 100 );
	self->s.v.velocity[1] =
		( g_globalvars.v_forward[1] * 900 + g_globalvars.v_up[1] * 100 );
	self->s.v.velocity[2] =
		( g_globalvars.v_forward[2] * 900 + g_globalvars.v_up[2] * 100 ) + 250;

//	self->s.v.velocity[0] = g_globalvars.v_forward[0] + g_globalvars.v_up
//	g_globalvars.v_up
//	SetVector( self->s.v.velocity, 0, 0, 0 );
	stuffcmd( self, "bf\n" );
	self->s.v.ammo_cells = self->s.v.ammo_cells - 23;
	self->s.v.impulse = 0;
}

void FlashTimer2(  )
{
	gedict_t *te;

	te = PROG_TO_EDICT( self->s.v.owner );
//	te->FlashTime = te->FlashTime - 0.6;
	te->FlashTime = te->FlashTime - 0.1;

/*	if ( te->FlashTime < 5 || ( self->gren_eff_time <= g_globalvars.time && tg_data.gren_time ) )
	{
		te->FlashTime = 0;
		stuffcmd( te, "v_cshift; wait; bf\n" );
		ent_remove( self );
		return;
	}*/
	if ( te->FlashTime < 0 ) {
		te->FlashTime = 0;
		stuffcmd( te, "v_cshift; wait; bf\n" );
		ent_remove( self );
		return;
	}

	if ( te->FlashTime < 1.7 ) {
		stuffcmd( te, "v_cshift %.0f %.0f %.0f %.0f\n", te->FlashTime * 150,
		  te->FlashTime * 150, te->FlashTime * 150, te->FlashTime * 150 );
	}
	if ( te->FlashTime > 1.7 )
		stuffcmd ( te, "v_cshift 255 255 255 255\n" );

	self->s.v.nextthink = g_globalvars.time + 0.1;
}

void FlashGrenadeExplode2 ( )
{
	PROG_TO_EDICT( self->s.v.owner )->has_syringe = 1;
	self->s.v.effects = ( int ) self->s.v.effects | EF_BRIGHTLIGHT;
	trap_WriteByte ( MSG_MULTICAST, 23 );
	trap_WriteByte ( MSG_MULTICAST, 4 );
	trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[0] );
	trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[1] );
	trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[2] );
	trap_multicast( PASSVEC3( self->s.v.origin ), 1 );
	if ( other->FlashTime == 0 ) {
		newmis = spawn ( );
		newmis->s.v.classname = "timer";
		newmis->s.v.netname = "FlashTimer2";
		newmis->team_no = self->team_no;
		newmis->s.v.owner = EDICT_TO_PROG( other );
		newmis->s.v.think = ( func_t ) FlashTimer2;
		newmis->s.v.nextthink = g_globalvars.time + .1;
	}
	other->FlashTime = FLASHTIME_MINE;
	stuffcmd ( other, "v_cshift 255 255 255 255\n" );
}