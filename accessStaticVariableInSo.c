/*****************************************************************************
 *
 * Description: Get the real address of static struct ci_cpi_system_flag by dlopen() and dlsym()
 *
 *****************************************************************************/
static phTcomError_t initSymbolAddress(phTcomId_t testerID)
{
    phLogTesterMessage(testerID->loggerId, LOG_VERBOSE, "going to load static struct ci_cpi_system_flag");
    phTcomError_t phRtnValue = PHTCOM_ERR_OK;
    char *szError = NULL;
    char *szLibPath = HP83000_ROOT"/prod_com/sh_lib/libcicpi.so";       /*lib path*/
    void *pci_function_commandset = NULL;                                   /*the data section address in memory after loading lib, used for address caculation*/
    void *plibcicpi = dlopen(szLibPath, RTLD_LAZY);

    if( NULL == plibcicpi )
    {
        /*dlopen failed*/
        phLogTesterMessage(testerID->loggerId, PHLOG_TYPE_ERROR, "failed to load libcicpi.so");
        szError = dlerror();
        if( NULL != szError )
        {
            phLogTesterMessage(testerID->loggerId, PHLOG_TYPE_ERROR, szError);
        }
        phRtnValue=PHTCOM_ERR_ERROR;
    }
    else
    {
        /*dlopen success*/
        /*get global value address in memory after libcicpi.so loaded, because it stored in the data section which is same with static struct ci_cpi_system_flag*/
        pci_function_commandset = dlsym(plibcicpi, "ci_function_commandset");
        if(NULL == pci_function_commandset)
        {
            /*dlsym failed*/
            phLogTesterMessage(testerID->loggerId, PHLOG_TYPE_ERROR, "failed to execute dlsym()");
            szError = dlerror();
            if( NULL != szError )
            {
                phLogTesterMessage(testerID->loggerId, PHLOG_TYPE_ERROR, szError);
            }
            phRtnValue=PHTCOM_ERR_ERROR;
        }
        else
        {
            /*dlsym success*/
            void *pci_func_cmdset_lib_addr = NULL;                              /*data section address in lib*/
            void *pci_cpi_sys_flag_lib_addr = NULL;                             /*ci_cpi_system_flag struct array address in lib*/
            char szPopenCMD[MAX_CMD_LEN] = {0};

            /*get global value ci_function_commandset address in libcicpi.so*/
            sprintf(szPopenCMD, "nm %s|grep ci_function_commandset |awk '{print $1}'", szLibPath);
            FILE *pShellnm = popen(szPopenCMD, "r");
            char szRet[1024] = {0}; 
            if( NULL == pShellnm )
            {
                phRtnValue=PHTCOM_ERR_ERROR;
                phLogTesterMessage(testerID->loggerId, PHLOG_TYPE_ERROR, "popen cmd: %s failed!!!", szPopenCMD);
                return PHTCOM_ERR_ERROR;
            }
            else
            {
                fgets(szRet, sizeof(szRet), pShellnm);
                pci_func_cmdset_lib_addr = (void*)strtol(szRet, (char**)NULL, 16);
            }
            pclose(pShellnm);

            /*get static struct array ci_cpi_system_flag address and size in libcicpi.so*/
            memset(szPopenCMD, 0, MAX_CMD_LEN);
            memset(szRet, 0, 1024);
            sprintf(szPopenCMD, "readelf -s %s |grep ci_cpi_system_flag |awk '{print $2\":\"$3}'", szLibPath);
            FILE *pShellreadelf = popen(szPopenCMD, "r");
            if(NULL == pShellreadelf)
            {
                phRtnValue=PHTCOM_ERR_ERROR;
                phLogTesterMessage(testerID->loggerId, PHLOG_TYPE_ERROR, "popen cmd: %s failed!!!", szPopenCMD);
                dlclose(plibcicpi);
                return PHTCOM_ERR_ERROR;
            }
            else
            {
                int itemp = 0;                                                  /*the size of struct ci_cpi_system_flag*/
                fgets(szRet, sizeof(szRet), pShellreadelf);
                sscanf(szRet, "%p:%d", &pci_cpi_sys_flag_lib_addr, &itemp);
                sysflgSize = itemp / sizeof(struct copy_ci_cpi_system_flag);    /*get the array size*/
            }
            pclose(pShellreadelf );

            /*get real address of ci_cpi_system_flag in memory after lib loaded*/
            /*the offset = global value address in memory after lib loaded - global value address in lib*/
            /*the real address of ci_cpi_system_flag in memory = the static sruct array address in lib + offset*/
            psysflg = pci_cpi_sys_flag_lib_addr + (pci_function_commandset - pci_func_cmdset_lib_addr);
        }
        dlclose(plibcicpi);
    }

    /*get set_sysflag_i_nomsg function address in memory after lib loaded*/
    void *plibshmfl = dlopen(HP83000_ROOT"/prod_com/sh_lib/libshmfl.so", RTLD_LAZY);
    if( NULL == plibshmfl )
    {
        /*dlopen failed*/
        phLogTesterMessage(testerID->loggerId, PHLOG_TYPE_ERROR, "failed to load libshmfl.so");
        szError = dlerror();
        if( NULL != szError )
        {
            phLogTesterMessage(testerID->loggerId, PHLOG_TYPE_ERROR, szError);
        }
        phRtnValue=PHTCOM_ERR_ERROR;
    }
    else
    {
        /*dlopen success*/
        pfunc_set_sysflag_i_nomsg = dlsym(plibshmfl, "set_sysflag_i_nomsg");    /*get address in memory*/
        if(NULL == pfunc_set_sysflag_i_nomsg)
        {
            /*dlsym failed*/
            phLogTesterMessage(testerID->loggerId, PHLOG_TYPE_ERROR, "failed to execute dlsym()");
            szError = dlerror();
            if( NULL != szError )
            {
                phLogTesterMessage(testerID->loggerId, PHLOG_TYPE_ERROR, szError);
            }
            phRtnValue=PHTCOM_ERR_ERROR;
        }
        dlclose(plibshmfl);
    }

    return phRtnValue;
}

