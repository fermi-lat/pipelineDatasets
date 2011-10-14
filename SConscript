# -*- python -*-
# $Header$
# Version: pipelineDatasets-01-00-00
Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

locIncs = listFiles(['pipelineDatasets/*.h'])
libEnv.Tool('addLinkDeps', package='pipelineDatasets', toBuild='rootlib')

pipelineDatasetsRootcint = libEnv.Rootcint('pipelineDatasets/pipelineDatasets_rootcint',
                                       ['pipelineDatasets/pipelineDatasets.h',
                                        'pipelineDatasets/pruneTuple.h'],
                                       includes = [''], localIncludes=locIncs,
                                       packageName='pipelineDatasets')
libEnv['rootcint_node'] = pipelineDatasetsRootcint

pipelineDatasets = libEnv.RootDynamicLibrary('pipelineDatasets',
                                         listFiles(['src/*.cxx']) + ['pipelineDatasets/pipelineDatasets_rootcint.cxx'])

progEnv.Tool('pipelineDatasetsLib')
progEnv.Tool('registerTargets', package = 'pipelineDatasets',
             rootcintSharedCxts = [[pipelineDatasets, libEnv]],
             includes = listFiles(['pipelineDatasets/*.h']))



