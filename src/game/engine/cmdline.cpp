/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Command line parser.
 *
 * @copyright Remnant is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "cmdline.h"
#include "hooker.h"
#include <captainslog.h>
#include <string.h>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>

class SpecialClass
{
public:
    void Init()
    {
        DEFINE_CALL(func, 0x0047D5A8, void, const SpecialClass *);
        func(this);
    }

public:
    unsigned Value;
};

int Obfuscate(const char *string)
{
    DEFINE_CALL(func, 0x0047CFE4, int, const char *);
    return func(string);
}

int Read_Game_Options(const char *string)
{
    DEFINE_CALL(func, 0x0047DAD8, int, const char *);
    return func(string);
}

extern int &Scenario;
extern char &ScenPlayer;
extern char &ScenDir;
extern char &Whom;
extern SpecialClass &Special;
extern BOOL &Debug_Map;
extern BOOL &Debug_Unshroud;
extern int &WChatMode;
extern BOOL &Debug_Quiet;
extern char &GameToPlay;
extern char &MenuSelection;
extern BOOL &OfflineMode;

#define FROMINSTALL 0xD95C68A2
#define GAME_SERVER 2
#define GAME_CLIENT 3

BOOL Parse_Command_Line(int argc, char **argv)
{
    Scenario = 1;
    ScenPlayer = 0;
    ScenDir = 0;
    Whom = 0;
    Special.Init();
    Debug_Map = 0;
    Debug_Unshroud = 0;

    // Start at 1 as 0 is the program itself.
    for (int i = 1; i < argc; ++i) {
        char *arg = argv[i];
        arg = strupr(arg);
        bool from_install = false;

        if (Obfuscate(arg) == FROMINSTALL) {
            Special.Value |= 0x80;
            from_install = true;
        } else if (strcasecmp(arg, "-WCHAT") == 0) {
            WChatMode = 1;
        } else if (strcasecmp(arg, "NOSOUND") == 0) {
            Debug_Quiet = true;
        } else if (strcasecmp(arg, "-SERVER") == 0) {
            GameToPlay = GAME_SERVER;
            OfflineMode = false;
            Debug_Map = true;
            const char *logfile = nullptr;
#if LOGGING_LEVEL != LOGLEVEL_NONE
            char dirbuf[PATH_MAX];
            char curbuf[PATH_MAX];
            char prevbuf[PATH_MAX];

#ifdef PLATFORM_WINDOWS
            char *tmp = getenv("USERPROFILE");

            if (tmp != NULL) {
                strcpy(dirbuf, tmp);
                strcat(dirbuf, "\\Documents\\Remnant");
                mkdir(dirbuf);
                strcat(dirbuf, "\\");
            } else {
                GetModuleFileNameA(0, dirbuf, sizeof(dirbuf));

                // Get the path to the executable minus the actual filename.
                for (char *i = &dirbuf[strlen(dirbuf)]; i >= dirbuf && (*i != '\\' || *i != '/'); --i) {
                    *i = '\0';
                }
            }
#else
            char *homedir = getenv("HOME");
            if (homedir != nullptr) {
                strcpy(dirbuf, homedir);
            }

            if (homedir == nullptr) {
                homedir = getpwuid(getuid())->pw_dir;
                if (homedir != nullptr) {
                    strcpy(dirbuf, homedir);
                }
            }

            if (homedir != nullptr) {
                strcat(dirbuf, "/.config/Remnant");
            }
#endif
            const char *prefix = "";
            strcpy(prevbuf, dirbuf);
            strcat(prevbuf, prefix);
            strcat(prevbuf, "SoleServerDebugLogPrev.txt");
            strcpy(curbuf, dirbuf);
            strcat(curbuf, prefix);
            strcat(curbuf, "SoleServerDebugLogFile.txt");
            remove(prevbuf);
            rename(curbuf, prevbuf);
            logfile = curbuf;
#endif
            captainslog_init(LOGLEVEL_DEBUG, logfile, true, false, false);
            captainslog_ignoreasserts(false);
            captainslog_allowpopups(true);

        } else if (strcasecmp(arg, "-CLIENT") == 0) {
            Read_Game_Options("CLIENT.INI");
            MenuSelection = 1;
            const char *logfile = nullptr;
#if LOGGING_LEVEL != LOGLEVEL_NONE
            char dirbuf[PATH_MAX];
            char curbuf[PATH_MAX];
            char prevbuf[PATH_MAX];

#ifdef PLATFORM_WINDOWS
            char *tmp = getenv("USERPROFILE");

            if (tmp != NULL) {
                strcpy(dirbuf, tmp);
                strcat(dirbuf, "\\Documents\\Remnant");
                mkdir(dirbuf);
                strcat(dirbuf, "\\");
            } else {
                GetModuleFileNameA(0, dirbuf, sizeof(dirbuf));

                // Get the path to the executable minus the actual filename.
                for (char *i = &dirbuf[strlen(dirbuf)]; i >= dirbuf && (*i != '\\' || *i != '/'); --i) {
                    *i = '\0';
                }
            }
#else
            char *homedir = getenv("HOME");
            if (homedir != nullptr) {
                strcpy(dirbuf, homedir);
            }

            if (homedir == nullptr) {
                homedir = getpwuid(getuid())->pw_dir;
                if (homedir != nullptr) {
                    strcpy(dirbuf, homedir);
                }
            }

            if (homedir != nullptr) {
                strcat(dirbuf, "/.config/Remnant");
            }
#endif
            const char *prefix = "";
            strcpy(prevbuf, dirbuf);
            strcat(prevbuf, prefix);
            strcat(prevbuf, "SoleClientDebugLogPrev.txt");
            strcpy(curbuf, dirbuf);
            strcat(curbuf, prefix);
            strcat(curbuf, "SoleClientDebugLogFile.txt");
            remove(prevbuf);
            rename(curbuf, prevbuf);
            logfile = curbuf;
#endif
            captainslog_init(LOGLEVEL_DEBUG, logfile, true, false, false);
            captainslog_ignoreasserts(false);
            captainslog_allowpopups(true);
        }
    }

    return true;
}
