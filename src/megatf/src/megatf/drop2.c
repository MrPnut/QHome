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
 *  $Id: drop2.c,v 1.00 2006/09/15 XavioR Exp $
 */
#include "g_local.h"

// The toaster
// Toaster hits someone (no effect?)
void ToasterTouch ( )
{
	if ( other->s.v.takedamage ) {
		sound(self, 3, "shambler/smack.wav", 0.8, 1);
		return;
	}
	sound(self, 1, "weapons/bounce.wav", 1, 1);
}

// Electrocute whoever's in the water
void ToasterElec ( )
{
	gedict_t *te;

	self->s.v.effects = 4;
	sound(self, 1, "weapons/lhit.wav", 1, 1);
	if ( self->s.v.waterlevel > 0 )
	{
		te = findradius( world, self->s.v.origin, 950 );
		while ( te )
		{
			if ( streq( te->s.v.classname, "player" ) && te->s.v.waterlevel > 1 ) {
				if ( te->radsuit_finished < g_globalvars.time ) {
					mtf_deathmsg( 7 );
					TF_T_Damage( te, self, PROG_TO_EDICT( self->s.v.owner ), 250, 2, 0 );
				}
			}
			te = findradius( te, self->s.v.origin, 950 );
		}
		dremove( self );
	}
	else {
		dremove( self );
		return;
	}
}

// Throw Toaster
void Toaster ( )
{
	gedict_t *missile;
	if ( self->no_grenades_2 < 2 ) {
		G_sprint( self, 1, "Not enough type 2 grenades... get more\n" );
		return;
	}
	if (!tf_data.drop2)
	{
		G_sprint (self, 2, "Toasters have been disabled by the admin!\n");
		return;
	}
	self->no_grenades_2 = self->no_grenades_2 - 2;
	sound( self, CHAN_WEAPON, "misc/power.wav", 1, ATTN_NORM );
	missile = spawn(  );
	missile->s.v.owner = EDICT_TO_PROG( self );
	missile->s.v.movetype = 10;
	missile->s.v.solid = 2;
	missile->s.v.classname = "grenade";
	missile->s.v.effects = 8;

	// throw that toaster!
	makevectors( self->s.v.v_angle );
	if (self->s.v.v_angle[0]) {
		missile->s.v.velocity[0] =
		    g_globalvars.v_forward[0] * 600 + g_globalvars.v_up[0] * 200 +
		    crandom(  ) * g_globalvars.v_right[0] * 10 + crandom(  ) * g_globalvars.v_up[0] * 10;
		missile->s.v.velocity[1] =
		    g_globalvars.v_forward[1] * 600 + g_globalvars.v_up[1] * 200 +
		    crandom(  ) * g_globalvars.v_right[1] * 10 + crandom(  ) * g_globalvars.v_up[1] * 10;
		missile->s.v.velocity[2] =
		    g_globalvars.v_forward[2] * 600 + g_globalvars.v_up[2] * 200 +
		    crandom(  ) * g_globalvars.v_right[2] * 10 + crandom(  ) * g_globalvars.v_up[0] * 10;
	}
	else {
		//missile->s.v.velocity = aim(self, 10000);
		aim( self->s.v.velocity );	// = aim(self, 10000);
		missile->s.v.velocity[0] = missile->s.v.velocity[0] * 600;
		missile->s.v.velocity[1] = missile->s.v.velocity[1] * 600;
//		missile->s.v.velocity[2] = missile->s.v.velocity[2] * 600;
		missile->s.v.velocity[2] = 200;
	}
	SetVector( missile->s.v.avelocity, 300, 300, 300 );
	//missile.angles = vectoangles(missile.velocity);
	vectoangles( missile->s.v.velocity, missile->s.v.angles );
	missile->s.v.touch = ( func_t ) ToasterTouch;
	missile->s.v.nextthink = g_globalvars.time + 3;
	missile->s.v.think = ( func_t ) ToasterElec;
	setmodel( missile, "progs/v_spike.mdl" );
	setsize( missile, 0, 0, 0, 0, 0, 0 );
	setorigin( missile, self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2]);
}
/*
void W_FireRocket(  )
{
	if ( !tg_data.unlimit_ammo )
		self->s.v.currentammo = --( self->s.v.ammo_rockets );
	sound( self, CHAN_WEAPON, "weapons/sgun1.wav", 1, ATTN_NORM );
	KickPlayer( -2, self );
	newmis = spawn(  );
	g_globalvars.newmis = EDICT_TO_PROG( newmis );
	newmis->s.v.owner = EDICT_TO_PROG( self );
	newmis->s.v.movetype = MOVETYPE_FLYMISSILE;
	newmis->s.v.solid = SOLID_BBOX;

// set newmis speed     
	makevectors( self->s.v.v_angle );
//	VectorScale( g_globalvars.v_forward, 900, newmis->s.v.velocity );
	VectorScale( g_globalvars.v_forward, 1350, newmis->s.v.velocity );	// mtf speed is 1350

	vectoangles( newmis->s.v.velocity, newmis->s.v.angles );

	newmis->s.v.classname = "rocket";
	newmis->s.v.touch = ( func_t ) T_MissileTouch;
	newmis->voided = 0;
// set newmis duration
	newmis->s.v.nextthink = g_globalvars.time + 5;
	newmis->s.v.think = ( func_t ) SUB_Remove;
	newmis->s.v.weapon = 6;
	setmodel( newmis, "progs/missile.mdl" );
	setsize( newmis, 0, 0, 0, 0, 0, 0 );
	setorigin( newmis, self->s.v.origin[0] + g_globalvars.v_forward[0] * 8,
		   self->s.v.origin[1] + g_globalvars.v_forward[1] * 8,
		   self->s.v.origin[2] + g_globalvars.v_forward[2] * 8 + 16 );
}

void() W_LaunchStinger = 
{


	makevectors( self->s.v.angles );
	local entity missile;
	if (self.no_grenades_2 < 2)
	{
		sprint(self, 2, "Not enough type 2 grenades... get more\n");
		return;
	}
	if (!drop2)
	{
		sprint (self, 2, "Toasters have been disabled by the admin!\n");
		return;
	}
	self.no_grenades_2 = self.no_grenades_2 - 2;
	sound(self, TF_FLARE_OFF, "misc/power.wav", TF_FLARE_OFF, TF_FLARE_OFF);
	missile = spawn();
	missile.owner = self;
	missile.movetype = 10;
	missile.solid = 2;
	missile.classname = "grenade";
	missile.effects = 8;
	makevectors(self.v_angle);
	if (self.v_angle_x)
	{
		missile.velocity = v_forward * 600 + v_up * 200 + crandom() * v_right * 10 + crandom() * v_up * 10;
	}
	else
	{
		missile.velocity = aim(self, 10000);
		missile.velocity = missile.velocity * 600;
		missile.velocity_z = 200;
	}
	missile.avelocity = '300 300 300';
	missile.angles = vectoangles(missile.velocity);
	missile.touch = StingerTouch;
	missile.nextthink = time + 3;
	missile.think = StingerElec;
	setmodel(missile, "progs/v_spike.mdl");
	setsize(missile, '0 0 0', '0 0 0');
	setorigin(missile, self.origin);
}
*/