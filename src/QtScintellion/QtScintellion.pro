TEMPLATE = subdirs

SUBDIRS += \
    QtScintellionUserService \
    QtScintellionDatabase \
    QtScintellionRepository \
    QtScintellionUI

QtScintellionRepository.depends = QtScintellionDatabase
QtScintellionUserService.depends = QtScintellionRepository
QtScintellionUI.depends = QtScintellionUserService
