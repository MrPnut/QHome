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
 *  $Id: drop3.c,v 1.00 2006/09/15 20:12:21 XavioR Exp $
 */
#include "g_local.h"
// Drop3 functions
void	Prox ( );
void	HealSelf ( );
void	DetpackDisarm ( );
void	Drone ( );

// Drop3 selector function
void Drop3 ( )
{
	if (!tf_data.drop3)
	{
		G_sprint (self, 2, "Type 3 weapons disabled by Admin!\n");
		//self.impulse = 0;
		return;
	}
	if ( self->playerclass == PC_SCOUT )
		DetpackDisarm ( );
	if ( ( self->playerclass == PC_SNIPER ) && ( self->has_syringe == TRUE ) && ( self->tfstate & TFSTATE_AIMING ) )
		self->option = 9;
	if( self->playerclass == PC_SOLDIER )
		Prox ( );
	if ( self->playerclass == PC_MEDIC )
		HealSelf ( );
	if ( self->playerclass == PC_SPY ) {
		// took spycam out because it was never used anyways (not original to mtf either)
	}
	if ( self->playerclass == PC_ENGINEER )
		Drone ( );
	//self.impulse = 0; 
}