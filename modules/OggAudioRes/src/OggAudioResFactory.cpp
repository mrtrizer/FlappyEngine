#include "OggAudioResFactory.h"

#include <vector>

#include <vorbis/vorbisfile.h>

#include <ResRepositoryManager.h>
#include <IFileMonitorManager.h>
#include <OpenALAudioRes.h>
#include <ThreadManager.h>

namespace flappy {

OggAudioResFactory::OggAudioResFactory() {
    addDependency(ResRepositoryManager::id());
    addDependency(IFileMonitorManager::id());
    addDependency(ThreadManager::id());
}

// Based on https://www.gamedev.net/articles/programming/general-and-gameplay-programming/introduction-to-ogg-vorbis-r2031/
std::shared_ptr<ResBase> OggAudioResFactory::load(const std::string& name, ExecType) {
    const unsigned BUFFER_SIZE = 1024 * 32; // 32 KB buffers

    auto resMeta = manager<ResRepositoryManager>()->findResMeta(name);
    auto fileInfo = manager<ResRepositoryManager>()->findFileInfo(resMeta.data["input"]);
    std::string fullPath = fileInfo.path;
    manager<IFileMonitorManager>()->registerFile(fullPath);

    int endian = 0;             // 0 for Little-Endian, 1 for Big-Endian
    int bitStream;
    long bytes;
    char array[BUFFER_SIZE];    // Local fixed size array
    FILE *f;
    std::vector<char> buffer;
    ALenum format;
    ALsizei freq;

    // Open for binary reading
    f = fopen(fullPath.c_str(), "rb");

    vorbis_info *pInfo;
    OggVorbis_File oggFile;

    ov_open(f, &oggFile, NULL, 0);

    // Get some information about the OGG file
    pInfo = ov_info(&oggFile, -1);

    // Check the number of channels... always use 16-bit samples
    if (pInfo->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;
    // end if

    // The frequency of the sampling rate
    freq = pInfo->rate;

    do {
        // Read up to a buffer's worth of decoded sound data
        bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
        // Append to end of buffer
        buffer.insert(buffer.end(), array, array + bytes);
    } while (bytes > 0);

    ov_clear(&oggFile);

    auto rootEntity = manager<ThreadManager>()->entity();
    return std::make_shared<OpenALAudioRes>(rootEntity, std::move(buffer), format, freq);
}

std::shared_ptr<ResBase> OggAudioResFactory::create(const std::string&) {
    ALenum format = AL_FORMAT_MONO8;
    ALsizei freq = 22050;
    std::vector<char> buffer(2205);
    auto rootEntity = manager<ThreadManager>()->entity();
    return std::make_shared<OpenALAudioRes>(rootEntity, std::move(buffer), format, freq);
}

bool OggAudioResFactory::changed(const std::string & name) {
    auto resMeta = manager<ResRepositoryManager>()->findResMeta(name);
    auto fileInfo = manager<ResRepositoryManager>()->findFileInfo(resMeta.data["input"]);
    return manager<IFileMonitorManager>()->changed(fileInfo.path);
}

} // flappy
