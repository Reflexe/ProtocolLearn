TEMPLATE = subdirs
SUBDIRS = utilities protocols libraries examples
CONFIG += ordered

DISTFILES += \
    README.md \
    TODO.md \
    scripts/createprotocol.py \
    scripts/createtype.sh
