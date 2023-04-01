#include "ImageTexture.h"
#include <variant>
#include "entrePortaisEngine/tasks/ITask.hpp"
#include "entrePortaisEngine/tasks/LambdaTask.hpp"
#include "entrePortaisEngine/tasks/TaskManager.hpp"
#include "glad/glad.h"


void makeCheckImage(int checkImageWidth, int checkImageHeight, GLubyte *checkImage)
{
    int i, j, c;

    // Make a checkered pattern
    // This is very similar to the code in the OpenGL Programming Guide
    // And should be generated only once
    // It will be used to create a texture missing texture


    for (i = 0; i < checkImageHeight; i++) {
        for (j = 0; j < checkImageWidth; j++) {
            c = ((((i&0x8)==0)^(((j&0x8))==0)))*255;

            checkImage[i * checkImageHeight * 4 + j * 4 + 0] = static_cast<GLubyte>(c);
            checkImage[i * checkImageHeight * 4 + j * 4 + 1] = static_cast<GLubyte>(0);
            checkImage[i * checkImageHeight * 4 + j * 4 + 2] = static_cast<GLubyte>(c);
            checkImage[i * checkImageHeight * 4 + j * 4 + 3] = static_cast<GLubyte>(255);
        }
    }
}

bool checkImageLoaded = false;
GLubyte checkImage[IMAGE_MISSING_SIZE * IMAGE_MISSING_SIZE * 4];

GLubyte *getCheckImage() {
    if (!checkImageLoaded) {
        makeCheckImage(IMAGE_MISSING_SIZE, IMAGE_MISSING_SIZE, checkImage);
        checkImageLoaded = true;
    }
    return checkImage;
}




unsigned int entre_portais::ImageTexture::GetID() const {
    return texture_id_;
}

ImTextureID entre_portais::ImageTexture::GetImTextureID() const {
    return reinterpret_cast<ImTextureID>(texture_id_);
}

std::string entre_portais::ImageTexture::GetName() const {
    return name_;
}

void entre_portais::ImageTexture::initialize() {
    is_initialized_ = true;
}

void entre_portais::ImageTexture::load() {
    bool can_load = task_handler_.getStatus() == TaskStatus::SUCCESS;
    if (!can_load) {
        return;
    }

    is_loaded_ = true;
}

entre_portais::ImageTexture::ImageTexture(std::string path, std::string name) {
    // Create the OpenGL texture
    createOpenGLTexture();

    // Create the task to load the image
    auto taskManager = TaskManager::getInstance();
    unsigned int texture_id = texture_id_;
    task_handler_ = taskManager->addTask<LambdaTask<EmptyState>>([path, texture_id](EmptyState &state) -> TaskRunResult {
        // Load the image
        int width, height, nrChannels;

        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
        if (data){
            // Create the sync task to load the image
            auto taskManager = TaskManager::getInstance();

            taskManager->addSyncTask<LambdaTask<EmptyState>>([texture_id, data, width, height](EmptyState &state) -> TaskRunResult {
                // Save the texture that was bound before
                int previous_texture;
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_texture);

                glBindTexture(GL_TEXTURE_2D, texture_id);

                // Set the texture data
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                // Set the old texture back
                glBindTexture(GL_TEXTURE_2D, previous_texture);

                // Free the image data
                stbi_image_free(data);

                return TaskRunResult::SUCCESS;
            }, std::string("Loading image IN OPENGL " + path), EmptyState {});
        }
        else {
            spdlog::error("Failed to load texture {}", path);
        }
        return TaskRunResult::SUCCESS;
    }, std::string("Loading image " + path), EmptyState {});


}

void entre_portais::ImageTexture::createOpenGLTexture() {
     // Create the OpenGL texture
    glGenTextures(1, &texture_id_);
    glActiveTexture(GL_TEXTURE31);
    // Save the texture that was bound before
    int previous_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_texture);

    glBindTexture(GL_TEXTURE_2D, texture_id_);

    // Set the texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set temporary texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IMAGE_MISSING_SIZE, IMAGE_MISSING_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, getCheckImage());
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set the old texture back
    glBindTexture(GL_TEXTURE_2D, previous_texture);
}

bool entre_portais::ImageTexture::isLoaded() const {
    // Check if the image is already loaded
    return (task_handler_.getStatus() == TaskStatus::SUCCESS);
}
