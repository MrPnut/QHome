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
 *  $Id: map_ents.c,v 1.00 2006/09/15 XavioR Exp $
 */
#include "g_local.h"

void train_blocked();
void train_use();
void func_train_find();
//void blocker_use();

/*=====================
		MegaTF Map Entities
			===================*/
// jumppad (engbatxr, etc)
void trigger_jumper_touch ( )
{
	gedict_t *finder;

	if ( !Activated( self, other ) )
	{
		if ( self->else_goal )
		{
			finder = Findgoal( self->else_goal );
			if ( finder)
				DoResults ( finder, other, ( self->goal_result & 2));
		}
		return;
	}

	if ( other->s.v.classname == "grenade" || !streq( other->s.v.classname, "player" ) )
		return;
	else
	{
		if ( other->s.v.health > 0 && !self->armorclass )
		{
			other->s.v.velocity[2] = self->speed * 7;
			if ( streq( other->s.v.classname, "player" ) )
			{
				if ( other->fly_sound < g_globalvars.time )
				{
					other->fly_sound = g_globalvars.time + 1.5;
					if ( !( ( int ) self->s.v.spawnflags & 2 ) )
						sound ( other, 0, "misc/boing.wav", 1, 1);
				}
			}
		}
	}
	if ( (int) self->s.v.spawnflags & 1 )
		dremove ( self );
}

void trigger_jumper ( )
{
	if ( CheckExistence ( ) == 0 )
	{
		dremove ( self);
		return;
	}

	InitTrigger ( );
	trap_precache_sound ( "misc/boing.wav");
	self->s.v.touch = ( func_t ) trigger_jumper_touch;
	if ( !self->speed )
		self->speed = 1000;
}

/*
		Glass
					*/
void blocker_use() 
{
	if (!(self->state ))
	{
		self->state  = 1;
		setorigin(self, self->s.v.origin[0] - 8000,self->s.v.origin[1] - 8000,self->s.v.origin[2] - 8000);// '8000 8000 8000');
		sound(self, 2, self->s.v.noise1, 1, 1);
	}
	else
	{
		self->state  = 0;
		setorigin(self, self->s.v.origin[0] + 8000,self->s.v.origin[1],self->s.v.origin[2]);//'8000 8000 8000');
		sound(self, 2, self->s.v.noise, 1, 1);
	}
}

void glass_pain(gedict_t* attacker, float damage) 
{
	if ((int)(self->s.v.spawnflags) & 8)
	{
		self->s.v.health = self->s.v.max_health;
	}
}

void glass_die() 
{
	gedict_t* new;
	vec3_t tmpvec;
	while (self->color > 0)
	{
		new = spawn();
		VectorCopy(self->s.v.origin,new->s.v.origin);
		if (g_random() < 0.5)
		{
			setmodel(new, "progs/glass2.mdl");
		}
		else
		{
			setmodel(new, "progs/glass1.mdl");
		}
		setsize(new ,0, 0, 0 ,0, 0, 0);
		if (self->height != 100)
		{
			new->s.v.velocity[0] = 70 * crandom();
			new->s.v.velocity[1] = 70 * crandom();
			new->s.v.velocity[2] = 140 + 70 * g_random();
		}
		else
		{
			new->s.v.velocity[0] = 400 * crandom();
			new->s.v.velocity[1] = 400 * crandom();
			new->s.v.velocity[2] = 140 + 70 * g_random();
		}
		new->s.v.movetype = MOVETYPE_BOUNCE;
		new->s.v.solid = SOLID_BBOX;
		new->s.v.avelocity[0] = g_random() * 600;
		new->s.v.avelocity[1] = g_random() * 600;
		new->s.v.avelocity[2] = g_random() * 600;
		new->s.v.nextthink = g_globalvars.time + 2 + g_random() * 3;
		new->s.v.think = (func_t) SUB_Remove;
		VectorAdd(self->s.v.origin, self->s.v.mins, self->s.v.absmin);
		VectorAdd( self->s.v.origin, self->s.v.maxs, self->s.v.absmax );
		tmpvec[0] = self->s.v.absmin[0] + g_random() * (self->s.v.absmax[0] - self->s.v.absmin[0]);
		tmpvec[1] = self->s.v.absmin[1] + g_random() * (self->s.v.absmax[1] - self->s.v.absmin[1]);
		tmpvec[2] = self->s.v.absmin[2] + g_random() * (self->s.v.absmax[2] - self->s.v.absmin[2]);
		setorigin(new, PASSVEC3(tmpvec));
		self->color = self->color - 1;
	}
	if (self->s.v.noise2)
	{
		if ( trap_pointcontents(PASSVEC3(self->s.v.origin)) == -3)
		{
			sound(self, 2, "effects/rcksplsh.wav", 1, 1);
		}
		else
		{
			sound(self, 2, self->s.v.noise2, 1, 1);
		}
	}
 ent_remove(self);
}

//glass (windows)
void func_glass() 
{
	vec3_t tmpvec;
	self->s.v.movetype = MOVETYPE_PUSH;
	self->s.v.solid = SOLID_BSP;
	self->mdl = self->s.v.model ;
	setmodel(self, self->s.v.model );
	setsize(self, PASSVEC3( self->s.v.mins ), PASSVEC3( self->s.v.maxs ));
	setorigin(self, PASSVEC3(self->s.v.origin));
	self->s.v.model = "";
	trap_precache_sound("misc/null.wav");
	if (self->s.v.health > 0)
	{
		if (!(self->color))
		{
			VectorSubtract(self->s.v.maxs, self->s.v.mins, tmpvec);
			tmpvec[0]=tmpvec[0]*0.031;tmpvec[1]=tmpvec[1]*0.031;tmpvec[2]=tmpvec[2]*0.031;
			if (tmpvec[0] < 1)
			{
				tmpvec[0] = 1;
			}
			if (tmpvec[1] < 1)
			{
				tmpvec[1] = 1;
			}
			if (tmpvec[2] < 1)
			{
				tmpvec[2] = 1;
			}
			self->color = tmpvec[0] * tmpvec[1] * tmpvec[2];
		}
		else
		{
			if (self->color == -1)
			{
				self->color = 0;
			}
		}
		if (self->color > 16)
		{
			self->color = 16;
		}
		self->s.v.takedamage = 1;
		self->s.v.max_health = self->s.v.health;
		self->th_die = glass_die;
		self->th_pain = glass_pain;
	 trap_precache_model("progs/glass1.mdl");
	 trap_precache_model("progs/glass2.mdl");
	}
	if (self->s.v.target)
	{
		if (!(self->speed ))
		{
			self->speed  = 100;
		}
		if (!(self->dmg ))
		{
			self->dmg  = 2;
		}
		if (self->s.v.sounds == 1)
		{
			if (!(self->s.v.noise))
			{
				self->s.v.noise = "plats/train2.wav";
			}
			if (!(self->s.v.noise1))
			{
				self->s.v.noise1 = "plats/train1.wav";
			}
		 trap_precache_sound(self->s.v.noise);
		 trap_precache_sound(self->s.v.noise1);
		}
		self->cnt = 1;
		self->s.v.blocked = (func_t) train_blocked;
		self->s.v.use = ( func_t ) train_use;
		self->s.v.classname  = "train";
		self->s.v.think = (func_t) func_train_find;
		self->s.v.nextthink = self->s.v.ltime + 0.6;
	}
	else
	{
		self->s.v.use = (func_t) blocker_use;
		if ((int)(self->s.v.spawnflags) & 4)
		{
			self->state  = 0;
			setorigin(self, self->s.v.origin[0] + 800, self->s.v.origin[1] + 800, self->s.v.origin[2] + 800);// + '8000 8000 8000');
		}
		else
		{
			self->state  = 1;
			if (self->s.v.noise1)
			{
				sound(self, 2, self->s.v.noise1, 1, 1);
			}
		}
	}
	if (!(self->s.v.noise))
	{
		self->s.v.noise = "misc/null.wav";
	}
	if (!(self->s.v.noise1))
	{
		self->s.v.noise1 = "misc/null.wav";
	}
	if (!(self->s.v.noise2))
	{
		self->s.v.noise2 = "effects/shatter.wav";
	}
 trap_precache_sound(self->s.v.noise);
 trap_precache_sound(self->s.v.noise1);
 trap_precache_sound(self->s.v.noise2);
}

/*
		Ambient Sounds
							*/
void ambient_sound() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->s.v.noise))
	{
		G_Error("ambient_sound without noise");
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound(self->s.v.noise);
	if ((int)(self->s.v.spawnflags) == 4)
	{
	 trap_ambientsound(PASSVEC3(self->s.v.origin), self->s.v.noise, self->volume, 0);
	}
	else
	{
	 trap_ambientsound(PASSVEC3(self->s.v.origin), self->s.v.noise, self->volume, 1);
	}
}

void ambient_brook() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/brook.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/brook.wav", self->volume, 1);
}

void ambient_jungle() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/jungle.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/jungle.wav", self->volume, 1);
}

void ambient_nightpond() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/nitepond.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/nitepond.wav", self->volume, 1);
}

void ambient_alert() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/redalert.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/redalert.wav", self->volume, 1);
}

void ambient_onboard() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 0.6;
	}
 trap_precache_sound("ambience/onboard.wav");
	if (!(self->height))
	{
	 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/onboard.wav", self->volume, 0);
	}
	else
	{
	 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/onboard.wav", self->volume, 1);
	}
}

void ambient_unholy() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 0.8;
	}
 trap_precache_sound("ambience/unholy.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/unholy.wav", self->volume, 0);
}

void ambient_rocket_engine() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/onboard.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/onboard.wav", self->volume, 1);
}

void ambient_high_wind() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/2windy.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/2windy.wav", self->volume, 1);
}

void ambient_peakwind() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/peakwind.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/peakwind.wav", self->volume, 1);
}

void ambient_chant() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/chant.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/chant.wav", self->volume, 1);
}

void ambient_meadow() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/meadow.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/meadow.wav", self->volume, 1);
}

void ambient_ocean() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/ocean.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/ocean.wav", self->volume, 1);
}

void ambient_ice_moving() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/icemove.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/icemove.wav", self->volume, 1);
}

void ambient_eerie() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/eerie.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/eerie.wav", self->volume, 1);
}

void ambient_flagflap() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/flagflap.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/flagflap.wav", self->volume, 1);
}

void ambient_diesel() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/diesel.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/diesel.wav", self->volume, 1);
}

void ambient_chopper() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/chopper.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/chopper.wav", self->volume, 1);
}

void ambient_suck_wind() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
 trap_precache_sound("ambience/suck1.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/suck1.wav", 1, 3);
}

void ambient_drone() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/drone6.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/drone6.wav", self->volume, 2);
}

void ambient_flouro_buzz() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
 trap_precache_sound("ambience/buzz1.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/buzz1.wav", 1, 3);
}

void ambient_drip() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	if (!(self->volume))
	{
		self->volume = 1;
	}
 trap_precache_sound("ambience/drip1.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/drip1.wav", self->volume, 3);
}

void ambient_comp_hum() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
 trap_precache_sound("ambience/comp1.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/comp1.wav", 1, 3);
}

void ambient_thunder() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
 trap_precache_sound("ambience/thunder1.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/thunder1.wav", 1, 1);
}

void ambient_light_buzz() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
 trap_precache_sound("ambience/fl_hum1.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/fl_hum1.wav", 0.8, 3);
}

void ambient_swamp1() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
 trap_precache_sound("ambience/swamp1.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/swamp1.wav", 0.8, 3);
}

void ambient_swamp2() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
 trap_precache_sound("ambience/swamp2.wav");
 trap_ambientsound(PASSVEC3(self->s.v.origin), "ambience/swamp2.wav", 0.6, 3);
}

// SandBall!
void BallTouch() 
{
	//vec3_t vtemp;

	if (streq(other->s.v.classname, "player") && self->last_used <= g_globalvars.time)
	{
		sound(self, 3, "zombie/z_fall.wav", 0.8, 1);
		self->last_used = g_globalvars.time + 0.3;
	}
	if (strneq(other->s.v.classname, "player"))
	{
		sound(self, 1, "zombie/z_hit.wav", 1, 1);
	}
	if (streq(other->s.v.classname, "player"))
	{
		makevectors(other->s.v.v_angle);
		self->s.v.velocity[0] =
			other->s.v.velocity[0] + g_globalvars.v_forward[0] * 100 + g_globalvars.v_up[0] * 200;
		self->s.v.velocity[1] =
			other->s.v.velocity[1] + g_globalvars.v_forward[1] * 100 + g_globalvars.v_up[1] * 200;
		self->s.v.velocity[2] =
			other->s.v.velocity[2] + g_globalvars.v_forward[2] * 100 + g_globalvars.v_up[2] * 200;
		self->lastplayer = other;
	}
}

void BallCheck() 
{
	self->s.v.movetype = MOVETYPE_BOUNCE;
	self->s.v.nextthink = g_globalvars.time + 15;
	self->s.v.effects = 0;
}

void ballstart() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
}

void BallStart() 
{
	gedict_t* te;
	sound(self, 3, "sandball/whistle.wav", 1, 0);
	te = trap_find( world, FOFS( s.v.classname ), "ballstart" );
	while (te)
	{
		setorigin(self, PASSVEC3(te->s.v.origin));
		te = trap_find( te, FOFS( s.v.classname ), "ballstart" );
	}
	self->s.v.nextthink = g_globalvars.time + 5;
	self->s.v.think = (func_t) BallCheck;
	self->s.v.movetype = MOVETYPE_BOUNCE;
	self->s.v.velocity[2] = 40;
}

void ball() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
	trap_precache_model("progs/s_light.spr");
	setmodel(self, "progs/s_light.spr");
	self->s.v.movetype = MOVETYPE_NONE;
	setsize(self ,-16, -16, -14 ,16, 16, 10);
	self->s.v.solid = SOLID_TRIGGER;
	self->s.v.touch = ( func_t ) BallTouch;
	self->s.v.classname  = "ball";
	self->s.v.think = (func_t) BallCheck;
	self->s.v.nextthink = g_globalvars.time + 15;
	//self->s.v.oldorigin = self->s.v.origin;
	SetVector( self->s.v.oldorigin, self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] );
}

void GoalTouch() 
{
	gedict_t *te;
	te=world;
	if (strneq( other->s.v.classname, "ball" ) )
	{
		return;
	}
	if (self->last_used > g_globalvars.time)
	{
		return;
	}
	sound(self, 3, "sandball/goal.wav", 1, 0);
	self->last_used = g_globalvars.time + 10;
	te = trap_find( world, FOFS( s.v.classname ), "player" );
	while ( te )
	{
		if (self->team_no == 2)
		{
			CenterPrint(te, "Score for Reds by %s\n", other->lastplayer->s.v.netname);
		}
		else
		{
			CenterPrint(te, "Score for Blues by %s\n", other->lastplayer->s.v.netname);
		}
		if (te->team_no == self->team_no && self->team_no != 0)
		{
			te->real_frags = te->real_frags + self->s.v.frags;
			te->s.v.frags = te->s.v.frags + self->s.v.frags;
			te->s.v.health = te->s.v.max_health;
		}
		te = trap_find( te, FOFS( s.v.classname ), "player" );
	}
	other->s.v.nextthink = g_globalvars.time + 10;
	other->s.v.think = (func_t) BallStart;
	other->s.v.movetype = MOVETYPE_FLYMISSILE;
	other->s.v.velocity[2] = 40;
	other->s.v.effects = self->team_no * 64;
}

void sandball_goal() 
{
	if (CheckExistence() == 0)
	{
		dremove(self);
		return;
	}
 trap_precache_model("progs/null.mdl");
 trap_precache_sound("sandball/goal.wav");
 trap_precache_sound("sandball/whistle.wav");
 trap_precache_sound("zombie/z_fall.wav");
	setmodel(self, "progs/null.mdl");
	self->s.v.movetype = MOVETYPE_NONE;
	self->s.v.skin = self->team_no;
	self->s.v.solid = SOLID_TRIGGER;
	setsize(self ,-35, -35, -14 ,35, 35, 50);
	self->s.v.effects = 0;
	self->s.v.touch = (func_t) GoalTouch;
}