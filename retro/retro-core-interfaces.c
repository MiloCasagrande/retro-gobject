/* Retro  GObject libretro wrapper.
 * Copyright (C) 2014  Adrien Plazas
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

#include "retro-core-interfaces.h"

#include "retro-core-global.h"

gboolean retro_core_set_callback_interfaces (RetroCore *self, RetroEnvironmentCommand cmd, gpointer data) {
	switch (cmd) {
	case RETRO_ENVIRONMENT_COMMAND_GET_RUMBLE_INTERFACE:
		return retro_core_set_rumble_callback (self, (RetroRumbleCallback *) data);
	case RETRO_ENVIRONMENT_COMMAND_GET_SENSOR_INTERFACE:
		return FALSE;
	case RETRO_ENVIRONMENT_COMMAND_GET_CAMERA_INTERFACE:
		return FALSE;
	case RETRO_ENVIRONMENT_COMMAND_GET_LOG_INTERFACE:
		return retro_core_set_log_callback (self, (RetroLogCallback *) data);
	case RETRO_ENVIRONMENT_COMMAND_GET_PERF_INTERFACE:
		return FALSE;
	case RETRO_ENVIRONMENT_COMMAND_GET_LOCATION_INTERFACE:
		return FALSE;
	default:
		return FALSE;
	}
}

gboolean retro_core_set_rumble_callback (RetroCore *self, RetroRumbleCallback *cb) {
	RetroCore *global_self = retro_core_get_global_self ();
	gboolean interface_exists = global_self && retro_core_get_rumble_interface (global_self);
	if (!interface_exists) return FALSE;
	
	gboolean real_set_rumble_state (guint port, RetroRumbleEffect effect, guint16 strength) {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroRumble *interface = retro_core_get_rumble_interface (global_self);
			return RETRO_RUMBLE_GET_INTERFACE (interface)->set_rumble_state (interface, port, effect, strength);
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	cb->set_rumble_state = real_set_rumble_state;
	
	return TRUE;
}

gboolean retro_core_set_sensor_callback (RetroCore *self, RetroSensorCallback *cb) {
	RetroCore *global_self = retro_core_get_global_self ();
	gboolean interface_exists = global_self && retro_core_get_sensor_interface (global_self);
	if (!interface_exists) return FALSE;
	
	gboolean real_set_sensor_state (guint port, RetroSensorAction action, guint rate) {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroSensor *interface = retro_core_get_sensor_interface (global_self);
			return RETRO_SENSOR_GET_INTERFACE (interface)->set_sensor_state (interface, port, action, rate);
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	gfloat real_get_sensor_input (guint port, guint id) {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroSensor *interface = retro_core_get_sensor_interface (global_self);
			return RETRO_SENSOR_GET_INTERFACE (interface)->get_sensor_input (interface, port, id);
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	cb->set_sensor_state = real_set_sensor_state;
	cb->get_sensor_input = real_get_sensor_input;
	
	return TRUE;
}

gboolean retro_core_set_log_callback (RetroCore *self, RetroLogCallback *cb) {
	RetroCore *global_self = retro_core_get_global_self ();
	gboolean interface_exists = global_self && retro_core_get_log_interface (global_self);
	if (!interface_exists) return FALSE;
	
	gboolean real_log (guint level, const char *format, ...) {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroLog *interface = retro_core_get_log_interface (global_self);
			// FIXME pass the variable arguments
			return RETRO_LOG_GET_INTERFACE (interface)->log (interface, level, format);
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	cb->log = real_log;
	
	return TRUE;
}

