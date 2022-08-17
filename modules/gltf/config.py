def can_build(env, platform):
    # return env["tools"] and not env["disable_3d"]
    # return not env["disable_3d"
    return True


def configure(env):
    pass


def get_doc_classes():
    return [
        "EditorSceneImporterGLTF",
        "GLTFAccessor",
        "GLTFAnimation",
        "GLTFBufferView",
        "GLTFCamera",
        "GLTFDocument",
        "GLTFLight",
        "GLTFMesh",
        "GLTFNode",
        "GLTFSkeleton",
        "GLTFSkin",
        "GLTFSpecGloss",
        "GLTFState",
        "GLTFTexture",
        "PackedSceneGLTF",
    ]


def get_doc_path():
    return "doc_classes"
