

TRIBITS_PACKAGE(Triutils)

ADD_SUBDIRECTORY(src)

TRIBITS_ADD_TEST_DIRECTORIES(test)

#
# Exclude files for source package.
#

TRIBITS_EXCLUDE_FILES(
  src/stamp-h.in
  )

TRIBITS_PACKAGE_POSTPROCESS()
