TEMPLATE = subdirs

SUBDIRS += src \
    example
SUBDIRS += deps/qhttp

src.depends += deps/qhttp
