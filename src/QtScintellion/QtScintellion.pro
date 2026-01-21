TEMPLATE = subdirs

SUBDIRS += \
    QtScintellionTests \
    QtScintellionUserService \
    QtScintellionDatabase \
    QtScintellionRepository \
    QtScintellionUI

QtScintellionRepository.depends = QtScintellionDatabase
QtScintellionUserService.depends = QtScintellionRepository
QtScintellionUI.depends = QtScintellionUserService
QtScintellionTests.depends = QtScintellionUserService
