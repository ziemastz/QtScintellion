TEMPLATE = subdirs

SUBDIRS += \
    QtScintellionDatabase \
    QtScintellionRepository \
    QtScintellionUI

QtScintellionRepository.depends = QtScintellionDatabase
QtScintellionUI.depends = QtScintellionRepository
