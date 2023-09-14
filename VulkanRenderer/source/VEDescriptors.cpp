#include "VEDescriptors.h"

#include <cassert>
#include <stdexcept>

namespace VE
{

    /* ************************************************************************************************************************ */
    /* ***    Builder - VEDescriptorSetLayout   ******************************************************************************* */
    /* ************************************************************************************************************************ */

    VEDescriptorSetLayout::Builder::Builder(VEDevice& device)
        : veDevice(device)
    {

    }

    VEDescriptorSetLayout::Builder::~Builder()
    {

    }

    VEDescriptorSetLayout::Builder& VEDescriptorSetLayout::Builder::addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count)
    {
        assert(bindings.count(binding) == 0 && "Binding already in use");

        VkDescriptorSetLayoutBinding layoutBinding = {};
        layoutBinding.binding = binding;
        layoutBinding.descriptorType = descriptorType;
        layoutBinding.descriptorCount = count;
        layoutBinding.stageFlags = stageFlags;
        bindings[binding] = layoutBinding;

        return *this;
    }

    std::unique_ptr<VEDescriptorSetLayout> VEDescriptorSetLayout::Builder::build() const 
    {
        return std::make_unique<VEDescriptorSetLayout>(veDevice, bindings);
    }

    /* ************************************************************************************************************************ */
    /* ***    VEDescriptorSetLayout    **************************************************************************************** */
    /* ************************************************************************************************************************ */

    VEDescriptorSetLayout::VEDescriptorSetLayout(VEDevice& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
        : veDevice(device)
        , bindings(bindings)
    {
        std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {};
        for (auto kv : bindings)
        {
            setLayoutBindings.push_back(kv.second);
        }

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo = {};
        descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
        descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

        if (vkCreateDescriptorSetLayout(veDevice.device(), &descriptorSetLayoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }

    VEDescriptorSetLayout::~VEDescriptorSetLayout() 
    {
        vkDestroyDescriptorSetLayout(veDevice.device(), descriptorSetLayout, nullptr);
    }

    /* ************************************************************************************************************************ */
    /* ***    Builder - VEDescriptorPool    *********************************************************************************** */
    /* ************************************************************************************************************************ */

    VEDescriptorPool::Builder::Builder(VEDevice& device)
        : veDevice(device)
        , maxSets(1000)
        , poolFlags(0)

    {

    }

    VEDescriptorPool::Builder::~Builder()
    {

    }

    VEDescriptorPool::Builder& VEDescriptorPool::Builder::addPoolSize(VkDescriptorType descriptorType, uint32_t count)
    {
        poolSizes.push_back({ descriptorType, count });
        return *this;
    }

    VEDescriptorPool::Builder& VEDescriptorPool::Builder::setPoolFlags(VkDescriptorPoolCreateFlags flags)
    {
        poolFlags = flags;
        return *this;
    }

    VEDescriptorPool::Builder& VEDescriptorPool::Builder::setMaxSets(uint32_t count)
    {
        maxSets = count;
        return *this;
    }

    std::unique_ptr<VEDescriptorPool> VEDescriptorPool::Builder::build() const
    {
        return std::make_unique<VEDescriptorPool>(veDevice, maxSets, poolFlags, poolSizes);
    }

    /* ************************************************************************************************************************ */
    /* ***    VEDescriptorPool    ********************************************************************************************* */
    /* ************************************************************************************************************************ */

    VEDescriptorPool::VEDescriptorPool(VEDevice& device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes)
        : veDevice(device)
    {
        VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
        descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        descriptorPoolInfo.pPoolSizes = poolSizes.data();
        descriptorPoolInfo.maxSets = maxSets;
        descriptorPoolInfo.flags = poolFlags;

        if (vkCreateDescriptorPool(veDevice.device(), &descriptorPoolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    VEDescriptorPool::~VEDescriptorPool()
    {
        vkDestroyDescriptorPool(veDevice.device(), descriptorPool, nullptr);
    }

    bool VEDescriptorPool::allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const 
    {
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.pSetLayouts = &descriptorSetLayout;
        allocInfo.descriptorSetCount = 1;

        // Might want to create a "DescriptorPoolManager" class that handles this case, and builds
        // a new pool whenever an old pool fills up. But this is beyond our current scope
        if (vkAllocateDescriptorSets(veDevice.device(), &allocInfo, &descriptor) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    void VEDescriptorPool::freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const
    {
        vkFreeDescriptorSets(veDevice.device(), descriptorPool, static_cast<uint32_t>(descriptors.size()), descriptors.data());
    }

    void VEDescriptorPool::resetPool()
    {
        vkResetDescriptorPool(veDevice.device(), descriptorPool, 0);
    }

    /* ************************************************************************************************************************ */
    /* ***    VEDescriptorWriter    ********************************************************************************************* */
    /* ************************************************************************************************************************ */

    VEDescriptorWriter::VEDescriptorWriter(VEDescriptorSetLayout& setLayout, VEDescriptorPool& pool)
        : setLayout(setLayout)
        , pool(pool)
    {

    }

    VEDescriptorWriter::~VEDescriptorWriter()
    {

    }

    VEDescriptorWriter& VEDescriptorWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo)
    {
        assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

        auto& bindingDescription = setLayout.bindings[binding];

        assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");

        VkWriteDescriptorSet write = {};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.descriptorType = bindingDescription.descriptorType;
        write.dstBinding = binding;
        write.pBufferInfo = bufferInfo;
        write.descriptorCount = 1;

        writes.push_back(write);
        return *this;
    }

    VEDescriptorWriter& VEDescriptorWriter::writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo)
    {
        assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

        auto& bindingDescription = setLayout.bindings[binding];

        assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");

        VkWriteDescriptorSet write = {};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.descriptorType = bindingDescription.descriptorType;
        write.dstBinding = binding;
        write.pImageInfo = imageInfo;
        write.descriptorCount = 1;

        writes.push_back(write);
        return *this;
    }

    bool VEDescriptorWriter::build(VkDescriptorSet& set)
    {
        bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
        if (!success) 
        {
            return false;
        }
        overwrite(set);
        return true;
    }

    void VEDescriptorWriter::overwrite(VkDescriptorSet& set)
    {
        for (auto& write : writes)
        {
            write.dstSet = set;
        }

        vkUpdateDescriptorSets(pool.veDevice.device(), writes.size(), writes.data(), 0, nullptr);
    }

}  // namespace VE
