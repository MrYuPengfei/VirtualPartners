TEMPLATE = aux
INSTALLER = virtual_partners_installer
INPUT = $$PWD/config/config.xml $$PWD/packages
virtual_partners.input = INPUT
virtual_partners.output = $$INSTALLER
virtual_partners.commands = ../../bin/binarycreator -c $$PWD/config/config.xml -p $$PWD/packages ${QMAKE_FILE_OUT}
virtual_partners.CONFIG += target_predeps no_link combine
OTHER_FILES = README data 
