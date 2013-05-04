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
 *  $Id: mtf_sniper.c,v 1.00 2006/09/15 XavioR Exp $
 */
#include "g_local.h"

// MegaTF sniper functions

// Sniper Drop3 (airstrike)
void AirStrike_Explode ( );
void    s_explode1(  );
/*void    s_explode2(  );
void    s_explode3(  );
void    s_explode4(  );
void    s_explode5(  );
void    s_explode6(  );*/

void MushroomFlame ( )
{
	gedict_t *flame;

	flame = spawn( );
	flame->s.v.owner = EDICT_TO_PROG( self );
	flame->s.v.movetype = 9;
	flame->s.v.solid = SOLID_NOT;
	flame->s.v.classname = "flamerflame";
	flame->s.v.effects = 4;
	if (self->s.v.classname == "airstrike") {
		flame->s.v.velocity[0] = g_random(  ) * 1000;
	}
	flame->s.v.velocity[1] = g_random( ) * 1000;
	flame->s.v.velocity[2] = 200;
	flame->s.v.think = ( func_t ) s_explode1;
	flame->s.v.nextthink = g_globalvars.time + 0.3;
	setmodel(flame, "progs/s_explod.spr");
	setsize(flame, 0, 0, 0, 0, 0, 0);
	setorigin(flame, ( self->s.v.origin[0] + 0 ), ( self->s.v.origin[1] + 0 ), ( self->s.v.origin[2] + 16 ) );
}

void AirStrike ( )
{
	sound (self, 0, "weapons/airstrik.wav", 1, 0);
	self->s.v.think = ( func_t ) AirStrike_Explode;
	self->s.v.nextthink = ( func_t ) g_globalvars.time + 1.7;
}

void AirStrike_FlyBy ( )
{
	G_bprint (1, "INCOMING AIRSTRIKE!\n");
	sound(self, 0, "weapons/fighter.wav", 1, 0);
	self->s.v.think = ( func_t ) AirStrike;
	self->s.v.nextthink = g_globalvars.time + 5;
}

void AirStrike_Explode ( )
{
	trap_WriteByte( MSG_MULTICAST, 23);
	trap_WriteByte( MSG_MULTICAST, 3);
	trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[0] );
	trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[1] );
	trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[2] );
	trap_multicast( PASSVEC3( self->s.v.origin ), 1 );
	self->s.v.classname = "airstrike";
	MushroomFlame( );
	MushroomFlame( );
	MushroomFlame( );
	MushroomFlame( );
	trap_WriteByte( MSG_MULTICAST, 23);
	trap_WriteByte( MSG_MULTICAST, 10);
	trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[0] );
	trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[1] );
	trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[2] );
	trap_multicast( PASSVEC3( self->s.v.origin ), 1 );
	mtf_deathmsg( 43 );
	T_RadiusDamage(self, PROG_TO_EDICT( self->s.v.owner ), 300, world);
	dremove(self);
}

void AirStrike_Target ( vec3_t org ) 
{
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	newmis = spawn( );
	newmis->s.v.movetype = MOVETYPE_NONE;
	newmis->s.v.solid = SOLID_NOT;
	newmis->team_no = owner->team_no;
	newmis->s.v.owner = EDICT_TO_PROG( owner );
	setsize(newmis, 0, 0, 0, 0, 0, 1);
	setmodel(newmis, "progs/dot3.spr");
	newmis->s.v.effects = 8;
	org[2] = org[2] + 5;
	setorigin( newmis, PASSVEC3( org ) );
	newmis->s.v.nextthink = g_globalvars.time + 10 + ( g_random( ) * 10 );
	newmis->s.v.think = ( func_t ) AirStrike_FlyBy;
}



