#ifndef slic3r_GUI_ExtraRenderers_hpp_
#define slic3r_GUI_ExtraRenderers_hpp_

#include <wx/dataview.h>

#if wxUSE_MARKUP && wxCHECK_VERSION(3, 1, 1)
    #define SUPPORTS_MARKUP
#endif

// ----------------------------------------------------------------------------
// DataViewBitmapText: helper class used by BitmapTextRenderer
// ----------------------------------------------------------------------------

class DataViewBitmapText : public wxObject
{
public:
    DataViewBitmapText( const wxString &text = wxEmptyString,
                        const wxBitmap& bmp = wxNullBitmap) :
        m_text(text),
        m_bmp(bmp)
    { }

    DataViewBitmapText(const DataViewBitmapText &other)
        : wxObject(),
        m_text(other.m_text),
        m_bmp(other.m_bmp)
    { }

    void SetText(const wxString &text)      { m_text = text; }
    wxString GetText() const                { return m_text; }
    void SetBitmap(const wxBitmap &bmp)     { m_bmp = bmp; }
    const wxBitmap &GetBitmap() const       { return m_bmp; }

    bool IsSameAs(const DataViewBitmapText& other) const {
        return m_text == other.m_text && m_bmp.IsSameAs(other.m_bmp);
    }

    bool operator==(const DataViewBitmapText& other) const {
        return IsSameAs(other);
    }

    bool operator!=(const DataViewBitmapText& other) const {
        return !IsSameAs(other);
    }

private:
    wxString    m_text;
    wxBitmap    m_bmp;

    wxDECLARE_DYNAMIC_CLASS(DataViewBitmapText);
};
DECLARE_VARIANT_OBJECT(DataViewBitmapText)

// ----------------------------------------------------------------------------
// BitmapTextRenderer
// ----------------------------------------------------------------------------
#if ENABLE_NONCUSTOM_DATA_VIEW_RENDERING
class BitmapTextRenderer : public wxDataViewRenderer
#else
class BitmapTextRenderer : public wxDataViewCustomRenderer
#endif //ENABLE_NONCUSTOM_DATA_VIEW_RENDERING
{
public:
    BitmapTextRenderer(wxWindow* parent,
        wxDataViewCellMode mode =
#ifdef __WXOSX__
        wxDATAVIEW_CELL_INERT
#else
        wxDATAVIEW_CELL_EDITABLE
#endif

        , int align = wxDVR_DEFAULT_ALIGNMENT
#if ENABLE_NONCUSTOM_DATA_VIEW_RENDERING
    );
#else
        ) :
    wxDataViewCustomRenderer(wxT("DataViewBitmapText"), mode, align),
        m_parent(parent)
    {
#ifdef SUPPORTS_MARKUP
        m_markupText = nullptr;
#endif // SUPPORTS_MARKUP
    }
#endif //ENABLE_NONCUSTOM_DATA_VIEW_RENDERING

    ~BitmapTextRenderer();

#ifdef SUPPORTS_MARKUP
    void EnableMarkup(bool enable = true);
#endif // SUPPORTS_MARKUP

    bool SetValue(const wxVariant& value);
    bool GetValue(wxVariant& value) const;
#if ENABLE_NONCUSTOM_DATA_VIEW_RENDERING && wxUSE_ACCESSIBILITY
    virtual wxString GetAccessibleDescription() const override;
#endif // wxUSE_ACCESSIBILITY && ENABLE_NONCUSTOM_DATA_VIEW_RENDERING

    virtual bool Render(wxRect cell, wxDC* dc, int state) override;
    virtual wxSize GetSize() const override;

    bool        HasEditorCtrl() const override
    {
#ifdef __WXOSX__
        return false;
#else
        return true;
#endif
    }
    wxWindow*   CreateEditorCtrl(wxWindow* parent, wxRect labelRect, const wxVariant& value) override;
    bool        GetValueFromEditorCtrl(wxWindow* ctrl, wxVariant& value) override;
    bool        WasCanceled() const { return m_was_unusable_symbol; }

    void        set_can_create_editor_ctrl_function(std::function<bool()> can_create_fn) { can_create_editor_ctrl = can_create_fn; }

private:
    DataViewBitmapText  m_value;
    bool                m_was_unusable_symbol{ false };
    wxWindow*           m_parent{ nullptr };

    std::function<bool()>    can_create_editor_ctrl { nullptr };

#ifdef SUPPORTS_MARKUP
    class wxItemMarkupText* m_markupText;
#endif // SUPPORTS_MARKUP
};


// ----------------------------------------------------------------------------
// BitmapChoiceRenderer
// ----------------------------------------------------------------------------

class BitmapChoiceRenderer : public wxDataViewCustomRenderer
{
public:
    BitmapChoiceRenderer(wxDataViewCellMode mode =
#ifdef __WXOSX__
        wxDATAVIEW_CELL_INERT
#else
        wxDATAVIEW_CELL_EDITABLE
#endif
        , int align = wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL
    ) : wxDataViewCustomRenderer(wxT("DataViewBitmapText"), mode, align) {}

    bool SetValue(const wxVariant& value);
    bool GetValue(wxVariant& value) const;

    virtual bool Render(wxRect cell, wxDC* dc, int state) override;
    virtual wxSize GetSize() const override;

    bool        HasEditorCtrl() const override { return true; }
    wxWindow*   CreateEditorCtrl(wxWindow* parent, wxRect labelRect, const wxVariant& value) override;
    bool        GetValueFromEditorCtrl(wxWindow* ctrl, wxVariant& value) override;

    void        set_can_create_editor_ctrl_function(std::function<bool()> can_create_fn) { can_create_editor_ctrl = can_create_fn; }

private:
    DataViewBitmapText      m_value;
    std::function<bool()>   can_create_editor_ctrl { nullptr };
};


#endif // slic3r_GUI_ExtraRenderers_hpp_
