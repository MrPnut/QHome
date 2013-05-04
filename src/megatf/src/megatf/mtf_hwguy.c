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
 *  $Id: mtf_hwguy.c,v 1.00 2006/09/15 XavioR Exp $
 */
#include "g_local.h"
void	make_explosion ( );
// MTF HWGuy Functions

// 20mm cannon
// self->nojumptime < - fixes the bh w/ 20mm bug
void BigAssBullet ( vec3_t direction, float damage )
{
	vec3_t org, src, vtemp;

	makevectors( self->s.v.v_angle );
	src[0] = self->s.v.origin[0] + ( g_globalvars.v_forward[0] * 10 );
	src[1] = self->s.v.origin[1] + ( g_globalvars.v_forward[0] * 10 );
	src[2] = self->s.v.absmin[2] + ( self->s.v.size[2] * 0.7 );
	ClearMultiDamage ( );
	VectorScale( direction, 1500, vtemp );
	VectorAdd( vtemp, src, vtemp );
	traceline( PASSVEC3( src ), PASSVEC3( vtemp ), 0, self );
	//traceline( src, (src + (direction * 1500)), 0, self );
	if ( g_globalvars.trace_fraction != 1 )
		TraceAttack ( damage, direction );

	if ( PROG_TO_EDICT( g_globalvars.trace_ent )->s.v.takedamage ) {
		org[0] = g_globalvars.trace_endpos[0] - ( g_globalvars.v_forward[0] * 4 );
		org[1] = g_globalvars.trace_endpos[1] - ( g_globalvars.v_forward[1] * 4 );
		org[2] = g_globalvars.trace_endpos[2] - ( g_globalvars.v_forward[2] * 4 );
		//org = (trace_endpos - (v_forward * 4));
		SpawnBlood ( org, 9 );
	}
	else {
		//org = (trace_endpos - (v_forward * 4));
		org[0] = g_globalvars.trace_endpos[0] - ( g_globalvars.v_forward[0] * 4 );
		org[1] = g_globalvars.trace_endpos[1] - ( g_globalvars.v_forward[1] * 4 );
		org[2] = g_globalvars.trace_endpos[2] - ( g_globalvars.v_forward[0] * 4 );
		trap_WriteByte( MSG_BROADCAST, SVC_TEMPENTITY );
		trap_WriteByte( MSG_BROADCAST, TE_SPIKE );
		trap_WriteCoord( MSG_BROADCAST, org[0] );
		trap_WriteCoord( MSG_BROADCAST, org[1] );
		trap_WriteCoord( MSG_BROADCAST, org[2] );
		trap_multicast( PASSVEC3( g_globalvars.trace_endpos ), MULTICAST_PHS );
	}

	ApplyMultiDamage ( );
}

void W_FireBigAss ( )
{
	vec3_t dir, vtemp;

	if ( self->s.v.ammo_shells < 10 )
	{
		make_explosion ( );
//		G_sprint (self, 2, "Ammo chamber jammed!\n"); // I just don't like excess messages
		sound ( self, 1, "weapons/asscan4.wav", 1, 1 );
		stuffcmd ( self, "bf\n" );
		self->option4 = 0;
		return;
	}

	self->nojumptime = ( g_globalvars.time + 0.75 );		// stops the 10.30.98 20mm speed "bug"

	// "weapons/sgun1.wav" was the old sound, this is 11.11.04's sound
	sound( self, 1, "weapons/20mm.wav", 1, 1 );

	KickPlayer( -5, self );
	muzzleflash( );
	stuffcmd (self, "bf\n");
//	self.ammo_shells = (self.ammo_shells - 10);
//	self.currentammo = (self.ammo_shells - 10);
	//self->s.v.currentammo = self->s.v.ammo_shells - 10;
	self->s.v.ammo_shells = self->s.v.ammo_shells - 10;
	self->s.v.currentammo = self->s.v.ammo_shells;

	makevectors( self->s.v.v_angle );
	dir[0] = g_globalvars.v_forward[0];dir[1] = g_globalvars.v_forward[1];dir[2] = g_globalvars.v_forward[2];
	mtf_deathmsg( 46 );
	BigAssBullet( dir, 50 );
	EjectShell ( );

	//self->s.v.velocity = (self.velocity - (v_forward * 200));
	VectorScale( g_globalvars.v_forward, 200, vtemp );
	VectorSubtract( self->s.v.velocity, vtemp, vtemp );
	self->s.v.velocity[0] = vtemp[0];
	self->s.v.velocity[1] = vtemp[1];
	if ( vtemp[2] > 179.9 && self->s.v.button2 )
		vtemp[2] = vtemp[2] * 2.1;		// "fixes" 20mm jump when facing down
	self->s.v.velocity[2] = vtemp[2];

	if ( self->s.v.ammo_shells == 0 )
		self->s.v.ammo_shells = 1;
}

void BigAssCarry ( )
{
	vec3_t vtemp;

	makevectors ( PROG_TO_EDICT( self->s.v.owner )->s.v.v_angle );
	vtemp[0] = ( PROG_TO_EDICT( self->s.v.owner )->s.v.origin[0] + ( g_globalvars.v_forward[0] * 12 ) + 0 ) + ( g_globalvars.v_right[0] * 5 );
	vtemp[1] = ( PROG_TO_EDICT( self->s.v.owner )->s.v.origin[1] + ( g_globalvars.v_forward[1] * 12 ) + 0 ) + ( g_globalvars.v_right[1] * 5 );
	vtemp[2] = ( PROG_TO_EDICT( self->s.v.owner )->s.v.origin[2] + ( g_globalvars.v_forward[2] * 12 ) + 20 ) + ( g_globalvars.v_right[2] * 5 );

	setorigin (self, PASSVEC3( vtemp ) );
	VectorCopy( PROG_TO_EDICT( self->s.v.owner )->s.v.angles, self->s.v.angles );
	//self->s.v.angles = PROG_TO_EDICT( self->s.v.owner )->s.v.angles;
	self->s.v.think = ( func_t ) BigAssCarry;
	self->s.v.nextthink = g_globalvars.time + 0.02;
}