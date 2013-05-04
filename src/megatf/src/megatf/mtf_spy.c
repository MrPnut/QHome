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
 *  $Id: mtf_spy.c,v 1.00 2006/09/15 XavioR Exp $
 */
#include "g_local.h"

// MegaTF spy functions

// Ammo trap (steals ammo ;D)
void SpyPackRemove ( )
{
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	G_sprint( owner, 2, "Your Ammo Trap failed...\n" );
	dremove( self );
}

void SpyPackTouch ( )
{
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	if ( self->pausetime > g_globalvars.time )
		return;
	if ( !streq( other->s.v.classname, "player" ) )
		return;
	if ( other->s.v.health <= 0 )
		return;
	if ( other->team_no == self->team_no )
		return;

	if ( streq( other->s.v.classname, "player" ) )
	{
		// steal all his ammo!
		other->s.v.ammo_shells = 0;
		other->s.v.ammo_nails = 0;
		other->s.v.ammo_rockets = 0;
		other->s.v.ammo_cells = 0;
		other->s.v.currentammo = 0;
		other->no_grenades_1 = 0;
		other->no_grenades_2 = 0;
		G_sprint( other, 2, "Your ammo has been robbed!\n" );
		sound( self, 3, "weapons/lock4.wav", 1, 1 );
		G_sprint( owner, 2, "Your ammo trap worked!\n" );
		dremove( self );
	}
}

void SpyPack ( )
{
	gedict_t *te;

	te=world;
	te = trap_find( te, FOFS( s.v.classname ), "drop1" );
	while ( te )
	{
		if ( te->s.v.owner == EDICT_TO_PROG( self ) ) {
			G_sprint( self, 2, "1 ammo trap at a time!\n" );
			return;
		}
		te = trap_find( te, FOFS( s.v.classname ), "drop1" );
	}

	G_sprint( self, 2, "You set an Ammo Trap!\n" );
	sound( self, 3, "weapons/lock4.wav", 1, 1 );
	newmis = spawn(  );
	newmis->s.v.owner = EDICT_TO_PROG( self );
	newmis->s.v.enemy = EDICT_TO_PROG( self );
	newmis->s.v.health = g_globalvars.time;
	newmis->s.v.weapon = 0;
	newmis->s.v.movetype = MOVETYPE_TOSS;
	newmis->s.v.solid = SOLID_TRIGGER;
	newmis->team_no = self->team_no;

	makevectors( self->s.v.v_angle );
	if ( self->s.v.v_angle[0] )
	{
		//newmis->s.v.velocity = v_forward * 100 + v_up * 200;
		newmis->s.v.velocity[0] =
		    g_globalvars.v_forward[0] * 100 + g_globalvars.v_up[0] * 200;
		newmis->s.v.velocity[1] =
		    g_globalvars.v_forward[1] * 100 + g_globalvars.v_up[1] * 200;
		newmis->s.v.velocity[0] =
		    g_globalvars.v_forward[2] * 100 + g_globalvars.v_up[2] * 200;
	}
	else
	{
		aim( newmis->s.v.velocity );
		VectorScale( newmis->s.v.velocity, 100, newmis->s.v.velocity );
		newmis->s.v.velocity[2] = 200;
	}
	//newmis->s.v.avelocity = '0 300 0';
	SetVector( newmis->s.v.avelocity, 0, 300, 0 );
	setsize( newmis, 0, 0, 0, 0, 0, 0 );
	setorigin( newmis, PASSVEC3( self->s.v.origin ) );
	newmis->s.v.nextthink = g_globalvars.time + 180;
	newmis->s.v.think = ( func_t ) SpyPackRemove;
	newmis->s.v.touch = ( func_t ) SpyPackTouch;
	newmis->s.v.classname = "drop1";
	setmodel( newmis, "progs/backpack.mdl" );
}

