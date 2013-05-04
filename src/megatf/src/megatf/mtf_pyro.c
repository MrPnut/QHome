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
 *  $Id: mtf_pyro.c,v 1.00 2006/09/15 XavioR Exp $
 */
#include "g_local.h"

// MTF Pyro Functions
// Lava Pool
void RemoveLavaPool ( )
{
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	G_sprint( owner, 2, "Your Lava has cooled...\n" );
	sound( self, 4, "misc/null.wav", 0, 1 );
	owner->has_syringe = 1;
	dremove( self );
}

//someone touches the pool..
void LavaTouch ( )
{
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	if ( !streq( other->s.v.classname, "player" ) && other->s.v.classname != "bot" )
		return;
	if ( other->s.v.health <= 0 )
		return;
	if ( other->team_no < 1 )
		return;

	if ( other->s.v.takedamage ) {		// hit something that bleeds/takes damage
		mtf_deathmsg( 15 );
		TF_T_Damage( other, owner, owner, 2, 0, 16 );
		if ( g_random(  ) < 0.1 )
			sound( other, 0, "misc/vapeur2.wav", 0.4, 1 );
		else {
			if ( g_random(  ) > 0.6 )
				sound( other, 0, "player/inh2o.wav", 0.8, 1 );
		}

		if ( other->s.v.classname != "bot" )
			stuffcmd( other, "bf\n" );
		if ( other->team_no == owner->team_no )
			return;

		other->s.v.origin[0] = self->s.v.origin[0];
		other->s.v.origin[1] = self->s.v.origin[1];
	}
}

//someone touches the ball (pre-pool)
void LavaBallTouch ( )
{
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	if ( other == owner )		// you can't be harmed by your own lava pool
		return;

	sound( self, 1, "zombie/z_miss.wav", 1, 1 );
	if ( other->s.v.takedamage )
	{
		if ( !streq( other->s.v.classname, "player" ) )
			return;
		if ( other->s.v.health <= 0 )
			return;
		if ( other->team_no < 1 )
			return;
		if ( other->team_no == owner->team_no )
			return;

		mtf_deathmsg( 15 );
		TF_T_Damage( other, self, owner, 20, 0, 16 );
	}
}

//become that funkeh pool! :0
void LavaBecome ( )
{
	// there is no lava in water!
	if ( self->s.v.waterlevel > 1 )
		dremove( self );

	setmodel( self, "progs/lavapool.mdl" );
	SetVector( self->s.v.avelocity, 0, 0, 0 );
	self->s.v.effects = EF_DIMLIGHT;
	mtf_deathmsg( 15 );
	self->s.v.classname = "timer";
	sound( self, 4, "weapons/lavapool.wav", 1, 1 );
	self->s.v.angles[0] = 0;
	self->s.v.angles[1] = 0;
	self->s.v.angles[2] = 0;
	setsize( self, -12, -12, 0, 12, 12, 2 );
	self->s.v.nextthink = g_globalvars.time + 30;
	self->s.v.think = ( func_t ) RemoveLavaPool;
	self->s.v.touch = ( func_t ) LavaTouch;
}
// spawn the lava pool (called from drop1)
void LavaPool ( )
{
	// only one!
	if ( self->has_syringe != 1 ) {
		G_sprint( self, 2, "You get 1 Lava Pool at a time!\n" );
		return;
	}
	self->has_syringe = 0;		// why did Ambush make drop commands use this one var i do not know..

	sound( self, 3, "weapons/throw.wav", 1, 1 );
	newmis = spawn(  );
	newmis->s.v.classname = "timer";
	newmis->s.v.owner = EDICT_TO_PROG( self );
	newmis->team_no = self->team_no;
	newmis->s.v.health = g_globalvars.time;
	newmis->s.v.weapon = 0;
	newmis->s.v.movetype = 6;
	newmis->s.v.solid = 1;
	newmis->team_no = self->team_no;
	makevectors( self->s.v.v_angle );
	// set velocities
	if ( self->s.v.v_angle[0] ) {
		newmis->s.v.velocity[0] = g_globalvars.v_forward[0] * 400 + g_globalvars.v_up[0] * 200;
		newmis->s.v.velocity[1] = g_globalvars.v_forward[1] * 400 + g_globalvars.v_up[1] * 200;
		newmis->s.v.velocity[2] = g_globalvars.v_forward[2] * 400 + g_globalvars.v_up[2] * 200;
	}
	else
	{
		aim( newmis->s.v.velocity );
		VectorScale( newmis->s.v.velocity, 400, newmis->s.v.velocity );
		newmis->s.v.velocity[2] = 200;
	}
	SetVector( self->s.v.avelocity, 0, 0, 0 );
	setsize( newmis, 0, 0, 0, 0, 0, 0 );
	setorigin( newmis, PASSVEC3( self->s.v.origin ) );
	newmis->s.v.nextthink = g_globalvars.time + 2;
	newmis->s.v.think = ( func_t ) LavaBecome;
	newmis->s.v.touch = ( func_t ) LavaBallTouch;
	setmodel( newmis, "progs/lavaball.mdl" );
}
