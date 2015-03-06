/*----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#------  This File is Part Of : ----------------------------------------------------------------------------------------#
#------- _  -------------------  ______   _   --------------------------------------------------------------------------#
#------ | | ------------------- (_____ \ | |  --------------------------------------------------------------------------#
#------ | | ---  _   _   ____    _____) )| |  ____  _   _   ____   ____   ----------------------------------------------#
#------ | | --- | | | | / _  |  |  ____/ | | / _  || | | | / _  ) / ___)  ----------------------------------------------#
#------ | |_____| |_| |( ( | |  | |      | |( ( | || |_| |( (/ / | |  --------------------------------------------------#
#------ |_______)\____| \_||_|  |_|      |_| \_||_| \__  | \____)|_|  --------------------------------------------------#
#------------------------------------------------- (____/  -------------------------------------------------------------#
#------------------------   ______   _   -------------------------------------------------------------------------------#
#------------------------  (_____ \ | |  -------------------------------------------------------------------------------#
#------------------------   _____) )| | _   _   ___   ------------------------------------------------------------------#
#------------------------  |  ____/ | || | | | /___)  ------------------------------------------------------------------#
#------------------------  | |      | || |_| ||___ |  ------------------------------------------------------------------#
#------------------------  |_|      |_| \____|(___/   ------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Licensed under the GPL License --------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Copyright (c) Nanni <lpp.nanni@gmail.com> ---------------------------------------------------------------------------#
#- Copyright (c) Rinnegatamante <rinnegatamante@gmail.com> -------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Credits : -----------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Smealum for ctrulib and ftpony src ----------------------------------------------------------------------------------#
#- StapleButter for debug font -----------------------------------------------------------------------------------------#
#- Lode Vandevenne for lodepng -----------------------------------------------------------------------------------------#
#- Jean-loup Gailly and Mark Adler for zlib ----------------------------------------------------------------------------#
#- Special thanks to Aurelio for testing, bug-fixing and various help with codes and implementations -------------------#
#-----------------------------------------------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <3ds.h>
#include "include/luaplayer.h"
#include "include/ftp/ftp.h"

static int connfd;

static int lua_initFTP(lua_State *L) {
    int argc = lua_gettop(L);
    if (argc != 0) return luaL_error(L, "wrong number of arguments");
    ftp_init();
	connfd = -1;
    return 0;
}

static int lua_termFTP(lua_State *L) {
    int argc = lua_gettop(L);
    if (argc != 0) return luaL_error(L, "wrong number of arguments");
    ftp_exit();
    return 0;
}

static int lua_checkFTPcommand(lua_State *L){
	int argc = lua_gettop(L);
    if (argc != 0) return luaL_error(L, "wrong number of arguments");
	if(connfd<0)connfd=ftp_getConnection();
		else{
			int ret=ftp_frame(connfd);
			if(ret==1)
			{
				sprintf(shared_ftp,"Client has disconnected. Wait for next client...");
				connfd=-1;
			}
	}
	lua_pushstring(L, shared_ftp);
	return 1;
}

//Register our Socket Functions
static const luaL_Reg Socket_functions[] = {
  {"initFTP",						lua_initFTP},
  {"termFTP",						lua_termFTP},
  {"updateFTP",						lua_checkFTPcommand},
  {0, 0}
};

void luaSocket_init(lua_State *L) {
	lua_newtable(L);
	luaL_setfuncs(L, Socket_functions, 0);
	lua_setglobal(L, "Socket");
}