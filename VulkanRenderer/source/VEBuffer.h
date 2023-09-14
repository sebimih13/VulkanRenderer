#pragma once

#include "VEDevice.h"

namespace VE
{

    class VEBuffer
    {
    public:
        VEBuffer(VEDevice& device, VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment = 1);
        ~VEBuffer();

        /** not copyable */
        VEBuffer(const VEBuffer&) = delete;
        VEBuffer& operator = (const VEBuffer&) = delete;

        VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void unmap();

        void writeToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        void writeToIndex(void* data, int index);
        VkResult flushIndex(int index);
        VkDescriptorBufferInfo descriptorInfoForIndex(int index);
        VkResult invalidateIndex(int index);

        inline VkBuffer getBuffer() const { return buffer; }
        inline void* getMappedMemory() const { return mapped; }
        inline uint32_t getInstanceCount() const { return instanceCount; }
        inline VkDeviceSize getInstanceSize() const { return instanceSize; }
        inline VkDeviceSize getAlignmentSize() const { return instanceSize; }
        inline VkBufferUsageFlags getUsageFlags() const { return usageFlags; }
        inline VkMemoryPropertyFlags getMemoryPropertyFlags() const { return memoryPropertyFlags; }
        inline VkDeviceSize getBufferSize() const { return bufferSize; }

    private:
        static VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);

        VEDevice& veDevice;
        void* mapped;
        VkBuffer buffer;
        VkDeviceMemory memory;

        VkDeviceSize bufferSize;
        uint32_t instanceCount;
        VkDeviceSize instanceSize;
        VkDeviceSize alignmentSize;
        VkBufferUsageFlags usageFlags;
        VkMemoryPropertyFlags memoryPropertyFlags;
    };

}  // namespace VE
