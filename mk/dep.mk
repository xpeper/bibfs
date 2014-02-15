
DEP ?= gcc -M

.for dir in ${INCDIR}
DFLAGS += -I ${dir}
.endfor

.if ${DEP:T:Mgcc}
DFLAGS += -MT ${@:R}.o
DFLAGS += -ansi -pedantic
.endif

.for src in ${SRC} ${GEN} ${SRC}
CLEAN += ${BUILD}/${src:R}.mk
.endfor

.for src in ${SRC}
${BUILD}/${src:R}.mk: ${src}
.endfor

.for src in ${GEN}
${BUILD}/${src:R}.mk: ${BUILD}/${src}
.endfor

.for src in ${SRC} ${GEN}
${BUILD}/${src:R}.mk:
	${DEP} -o $@ ${DFLAGS} ${DFLAGS_${src}} -c ${.ALLSRC:M*.c}
.endfor

# This is worth some explanation.
#
# Here we generate dependencies as .mk files, and .include them. But .include
# is processed before rules are applied, so these .includes bring in dependencies
# generated from a previous run. For the sake of the first run, the generated
# dependencies are included only if they exist already.
#
# This works out okay, because if the generated dependency files do not exist
# at the time the .include is processed (i.e. this is the first run) then their
# content is moot, because everything would be built for the first time anyway.
#
# The generated dependency files would be outdated for the current run
# if a #include is added or removed in a .c file, but then the .c file itself
# would outdate its target, and so would be rebuilt regardless - leaving behind
# (now correct) generated dependencies for the next run.

.for src in ${SRC} ${GEN}
dep:: ${BUILD}/${src:R}.mk
.endfor

dep::
.for src in ${SRC} ${GEN}
.if exists(${BUILD}/${src:R}.mk)
.include "${BUILD}/${src:R}.mk"
.endif
.endfor

