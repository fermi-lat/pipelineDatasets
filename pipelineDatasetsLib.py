#$Header: 
def generate(env, **kw):
    if not kw.get('depsOnly', 0):
        env.Tool('addLibrary', library = ['pipelineDatasets'])
    env.Tool('addLibrary', library = env['rootLibs'])
    env.Tool('commonRootDataLib')
def exists(env):
    return 1;
