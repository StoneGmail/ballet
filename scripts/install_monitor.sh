#!/usr/bin/env bash
CUR_PATH=$(cd "$(dirname "$0")"; pwd)
CRONTAB_BAK_FILE="${CUR_PATH}/crontab_bak"
SHELL_PATH="/home/yht/projects/interface/scripts/"
SHELL_NAME="process_monitor.sh"
TASK_COMMAND="sh ${SHELL_PATH}${SHELL_NAME}"
#CRONTAB_TASK1="30 08 * * * ${TASK_COMMAND}"
#CRONTAB_TASK2="30 13 * * * ${TASK_COMMAND}"
#CRONTAB_TASK3="30 20 * * * ${TASK_COMMAND}"

# At every minute from 30 through 59 past hour 8 on Monday, Tuesday, Wednesday, Thursday, and Friday.
CRONTAB_TASK1="30-59/3 8 * * 1,2,3,4,5 ${TASK_COMMAND}"
# At every minute from 30 through 59 past hour 20 on Monday, Tuesday, Wednesday, Thursday, and Friday.
CRONTAB_TASK2="30-59/3 20 * * 1,2,3,4,5 ${TASK_COMMAND}"
# At every minute past every hour from 9 through 16 on Monday, Tuesday, Wednesday, Thursday, and Friday.
CRONTAB_TASK3="*/3 9-16 * * 1,2,3,4,5 ${TASK_COMMAND}"
# At every minute past every hour from 0 through 2 on Tuesday, Wednesday, Thursday, Friday, and Saturday.
CRONTAB_TASK4="*/3 0-2 * * 2,3,4,5,6 ${TASK_COMMAND}"
# At every minute from 0 through 40 past hour 2 on Tuesday, Wednesday, Thursday, Friday, and Saturday.
CRONTAB_TASK5="0-40/3 2 * * 2,3,4,5,6 ${TASK_COMMAND}"


create_crontab() {
    echo 'Create crontab task...'
    crontab -l >> ${CRONTAB_BAK_FILE} 2>/dev/null
    sed -i "/${SHELL_NAME}/d" ${CRONTAB_BAK_FILE}
    echo "${CRONTAB_TASK1}" >> ${CRONTAB_BAK_FILE}
    echo "${CRONTAB_TASK2}" >> ${CRONTAB_BAK_FILE}
    echo "${CRONTAB_TASK3}" >> ${CRONTAB_BAK_FILE}
    echo "${CRONTAB_TASK4}" >> ${CRONTAB_BAK_FILE}
    echo "${CRONTAB_TASK5}" >> ${CRONTAB_BAK_FILE}
    crontab ${CRONTAB_BAK_FILE}
    echo 'Complete'
}

create_crontab
