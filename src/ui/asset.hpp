#pragma once

#include <memory>
#include <string>

class QWidget;

namespace g::ui
{

class Asset
{
public:
    enum class AssetType
    {
        FBX,
        PNG,
        JPG,
        JPEG,
        TGA,
        BMP,
        DDS,
        WAV,
        MP3,
        OGG,
        FLAC,
        SHADER,
        MATERIAL,
        TEXTURE,
        FONT,
        SCENE,
        PREFAB,
        SCRIPT,
        UNKNOWN
    };

private:
    struct AssetContainerBase
    {
        explicit AssetContainerBase(const void* data) noexcept;
        AssetContainerBase(AssetContainerBase const&) = delete;
        AssetContainerBase(AssetContainerBase&& r) noexcept;
        AssetContainerBase& operator=(AssetContainerBase const&) = delete;
        AssetContainerBase& operator=(AssetContainerBase&& r) noexcept;

        virtual ~AssetContainerBase() = 0;
        virtual AssetType type() const = 0;

    protected:
        const void* _data = nullptr;
    };

    template <AssetType T>
    struct AssetContainer;

public:
    Asset(std::string_view file_path);
    virtual ~Asset();

    std::string name() const;
    AssetType type() const;

    QWidget* previewWidget();

private:
    std::string _file_path;
    std::unique_ptr<AssetContainerBase> _container = nullptr;
};

} // namespace g::ui
