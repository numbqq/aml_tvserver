/*
 * Copyright (c) 2014 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description: c++ file
 */

#define LOG_MOUDLE_TAG "TV"
#define LOG_CLASS_TAG "TvConfigManager"

#include "TvConfigManager.h"
#include "ConfigFile.h"

static CIniFile *pIniFile = NULL;
int LoadConfigFile(const char *file_name)
{
    if (pIniFile != NULL)
        delete pIniFile;

    pIniFile = new CIniFile();
    pIniFile->LoadFromFile(file_name);
    return 0;
}

int UnloadConfigFile()
{
    if (pIniFile != NULL)
        delete pIniFile;
    return 0;
}

int ConfigSetStr(const char *section, const char *key, const char *value)
{
    return pIniFile->SetString(section, key, value);
}

const char *ConfigGetStr(const char *section,  const char *key, const char *def_value)
{
    return pIniFile->GetString(section, key, def_value);
}

int ConfigGetInt(const char *section, const char *key, const int def_value)
{
    return pIniFile->GetInt(section, key, def_value);
}

int ConfigSetInt(const char *section, const char *key, const int value)
{
    pIniFile->SetInt(section, key, value);
    return 0;
}

float ConfigGetFloat(const char *section, const char *key, const float def_value)
{
    return pIniFile->GetFloat(section, key, def_value);
}

int ConfigSetFloat(const char *section, const char *key, const int value)
{
    pIniFile->SetFloat(section, key, value);
    return 0;
}
